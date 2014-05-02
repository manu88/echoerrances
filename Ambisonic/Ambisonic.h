//
//  Ambisonics.h
//  encoder~
//
//  Created by Manuel Deneu on 26/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __encoder___Ambisonics__
#define __encoder___Ambisonics__

#include "AmbisonicUtility.h"
#include "../AudioTools/AudioInternals.h"
#include "../AudioTools/FloatComputation.h"
#include "AmbisonicReverb.h"
#include "AmbisonicRotation.h"

//debug
#include "../PDObjects/Debug_pd.h"
#include "../Internal/align.h"

#include <cstring>


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*

    ENCODER

 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class AmbisonicEncoder : public AudioProcessorBase
{
public :

    // distance max d'une source -> la source est mutée
    // voir computeMultiplier()
    enum {MaxDistance = 8};

    AmbisonicEncoder(int order) :
    AudioProcessorBase(AudioProcessorInput),
    m_angle(0.),
    m_distance(0.),
    m_multiplier(1.),
    m_multiplierLine(1.,1.),
    m_angleLine(0. , 0.)
    {
        pimpl = AmbisonicUtility::getEncoder(order);
        AmbisonicUtility::retainInstance();
    }

    ~AmbisonicEncoder()
    {
        pimpl->release();
        AmbisonicUtility::getInstance()->inspectEncoderList();
        AmbisonicUtility::releaseInstance();
    }

    float** getEncodingMatrix() const
    {
        return pimpl->getEncodingMatrix();
    }

    int getOrder() const
    {
        return pimpl->getOrder();
    }

    int getHarmonicNumber() const
    {
        return pimpl->getHarmonicNumber();
    }

    /* **** **** **** */

    float getAngle() const
    {
        return m_angle;
    }

    void setAngle(float newAngle)
    {
        m_angle = newAngle;
        m_angleLine.changeDestination(newAngle);
    }

    /* **** **** **** */

    float getDistance() const
    {
        return m_distance;
    }

    void setDistance(float newDistance)
    {
        m_distance = fabsf( newDistance );
        computeMultiplier();
    }




    //encoder
    /*virtual*/ inline void process(float **ins, float **outs, int bufferSize)
    {

        const float mult = m_multiplierLine.incPosition();

        float** m_ins = new float*[1];
        float** m_outs = new float*[getHarmonicNumber()];

        m_ins[0] = (float*) aligned_alloc(16, sizeof(float)*bufferSize);
        for(int i=0; i<getHarmonicNumber(); i++)
            m_outs[i] = (float*) aligned_alloc(16, sizeof(float)*bufferSize);

//        memcpy(m_ins[0], ins[0], sizeof(float)*bufferSize);
        FloatComputation::copyVector(m_ins[0] , ins[0], bufferSize);

        // muted
        if (m_distance>=MaxDistance)
        {
            int i = 0;
            for(; i < getHarmonicNumber(); i++)
                FloatComputation::clearVector(m_outs[i], bufferSize);
        }

        // go !
        else
        {
            const int index = AudioTools::radianWrap(m_angleLine.incPosition() ) * CICM_1OVER2PI_RATIO;


            FloatComputation::multiplyByConstant(m_ins[0], m_outs[0],mult , bufferSize);

            for(int i = 1; i < getHarmonicNumber(); i++)
            {
                for(int j = 0; j < bufferSize; j++)
                {
                    m_outs[i][j] = getEncodingMatrix()[i][index] * m_ins[0][j] * mult;
                    //pdAssert( ( (outs[i][j] <=1.) && (outs[i][j] >=-1. ) ) , "val out of [-1;1] in encoder");


                }
            }
        }

        for(int i=0; i<getHarmonicNumber(); i++)
        {
            memcpy(outs[i], m_outs[i], sizeof(float)*bufferSize);
            aligned_free(m_outs[i]);
        }
        delete[] m_outs;

        aligned_free(m_ins[0]);
        delete[] m_ins;
    }



protected :

    void computeMultiplier()
    {
        // linéaire
        AudioTools::clipVal(m_distance, 0.0, MaxDistance);

        m_multiplier = 1. - (m_distance/MaxDistance);
        m_multiplierLine.changeDestination(m_multiplier);
    }

private:


    // pas utile
    virtual void internalPrepare()
    {
    }

    float                     m_angle;
    /* Distance virtuelle*/
    float                     m_distance;

    /*
      valeur [0;1] calculé depuis m_distance:
        - m_distance = 0 -> m_multiplier = 1; (on est au plus près)
        - m_distance >= MaxDistance ->m_multiplier = 0.0 (trop loin la source est muté)
        - entre les 2 : interpolation via la méthode computeMultiplier()

     */
    float                     m_multiplier;

    AudioTools::LinearInterPolator m_multiplierLine;
    AudioTools::LinearInterPolator m_angleLine;

    InternalAmbisonicEncoder* pimpl;
};


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*

    DECODER

 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

class AmbisonicDecoder : public AudioProcessorBase
{
public:
    AmbisonicDecoder(int order, int numOuts):
    AudioProcessorBase(AudioProcessorOutput)
    {
        pimpl = AmbisonicUtility::getDecoder(order, numOuts);
        AmbisonicUtility::retainInstance();

        m_vectorInput  = nullptr;
        m_vectorOutput = nullptr;
    }

    ~AmbisonicDecoder()
    {
        pimpl->release();
        AmbisonicUtility::getInstance()->inspectDecoderList();
        AmbisonicUtility::releaseInstance();

        deleteBuffers();

    }

    int getOrder() const
    {
        return pimpl->getOrder();
    }

    int getHarmonicNumber() const
    {
        return pimpl->getHarmonicNumber();
    }

    int getNumOuts() const
    {
        return pimpl->getNumberOfOutputs();
    }

    //decoder
    /*virtual*/ inline void process(float **ins, float **outs, int bufferSize)
    {

        for(int i = 0; i < bufferSize; i++)
        {
            for(int j = 0; j < getHarmonicNumber(); j++)    //NOTE FOR PROGRAMING : getHarmonicNumber() = 7
            {
                m_vectorInput[j] = ins[j][i];
            }

            FloatComputation::matrixByVector( const_cast<const float**>( pimpl->getDecodingMatrix() ),
                                                                         m_vectorInput,
                                                                         m_vectorOutput,
                                                                         getNumOuts(),
                                                                         getHarmonicNumber()
                                                                        );
            for(int j = 0; j < getNumOuts(); j++)
            {
                outs[j][i] = m_vectorOutput[j];

               // pdAssert( ( (outs[j][i] <=1.) && (outs[j][i] >=-1. ) ) , "val out of [-1;1] in Decoder");
            }
        }
    }





private:

    virtual void internalPrepare()
    {
        deleteBuffers();

        //m_vectorInput  = new float[pimpl->getHarmonicNumber()];
        m_vectorInput = (float*) aligned_alloc(16, sizeof(float)*pimpl->getHarmonicNumber());

        //m_vectorOutput = new float[pimpl->getHarmonicNumber()];
        m_vectorOutput = (float*) aligned_alloc(16, sizeof(float)*pimpl->getHarmonicNumber());
    }

    void deleteBuffers()
    {
        if(m_vectorInput)
        {
            //delete[] m_vectorInput;
            aligned_free(m_vectorInput);
            m_vectorInput = nullptr;
        }
        if(m_vectorOutput)
        {
            //delete[] m_vectorOutput;
            aligned_free(m_vectorOutput);
            m_vectorOutput = nullptr;
        }
    }
    InternalAmbisonicDecoder* pimpl;

    float *m_vectorInput;
    float *m_vectorOutput;
};

#endif /* defined(__encoder___Ambisonics__) */
