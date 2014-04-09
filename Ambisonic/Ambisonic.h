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
#include "AudioInternals.h"
#include "FloatComputation.h"
#include "AmbisonicReverb.h"

//debug
#include "Debug_pd.h"

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class AmbisonicEncoder : public AudioProcessorBase
{
public :
    
    // distance max d'une source -> la source est mutée
    // voir computeMultiplier()
    enum {MaxDistance = 8};
    
    AmbisonicEncoder(int order) :
    m_angle(0.),
    m_distance(0.),
    m_multiplier(1.)
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
    
    
    
    virtual inline void process(float **ins, float **outs, int bufferSize)
    {
        int index = AudioTools::radianWrap(m_angle) * CICM_1OVER2PI_RATIO;
        
//        FloatComputation::copyVector(ins[0] , outs[0],  bufferSize);
        FloatComputation::multiplyByConstant(ins[0], outs[0], m_multiplier, bufferSize);
        
        int i = 1;
        
        for(; i < getHarmonicNumber(); i++)
        {
            int j = 0;
            for(; j < bufferSize; j++)
            {
                outs[i][j] = getEncodingMatrix()[i][index] * ins[0][j];
            }
        }
    }
    
    // pas utile
    virtual void prepare()
    {
    }
protected :
    
    void computeMultiplier()
    {
        // linéaire
        AudioTools::clipVal(m_distance, 0.0, MaxDistance);

        m_multiplier = 1. - (m_distance/MaxDistance);
    }
    
private:
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
    InternalAmbisonicEncoder* pimpl;
};


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class AmbisonicDecoder : public AudioProcessorBase
{
public:
    AmbisonicDecoder(int order, int numOuts)
    {
        pimpl = AmbisonicUtility::getDecoder(order, numOuts);
        AmbisonicUtility::retainInstance();
        
        m_vectorInput  = NULL;
        m_vectorOutput = NULL;
    }
    
    ~AmbisonicDecoder()
    {
        pimpl->release();
        AmbisonicUtility::getInstance()->inspectDecoderList();
        AmbisonicUtility::releaseInstance();

        deleteBuffersIfNeeded();

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

    virtual inline void process(float **ins, float **outs, int bufferSize)
    {
        for(int i = 0; i < bufferSize; i++)
        {
            for(int j = 0; j < getHarmonicNumber(); j++)
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
            }
        }
    }

    virtual void prepare()
    {
        deleteBuffersIfNeeded();
        
        m_vectorInput  = new float[pimpl->getHarmonicNumber()];
        m_vectorOutput = new float[pimpl->getHarmonicNumber()];
    }
    


    
private:
    
    void deleteBuffersIfNeeded()
    {
        if (m_vectorInput!=NULL)
            delete[] m_vectorInput;
        
        if (m_vectorOutput!=NULL)
            delete[] m_vectorOutput;
    }
    InternalAmbisonicDecoder* pimpl;
    
    float *m_vectorInput;
    float *m_vectorOutput;
};

#endif /* defined(__encoder___Ambisonics__) */
