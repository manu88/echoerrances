//
//  Binaural.cpp
//  encoder~
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "Binaural.h"
#include "BinauralUtility.h"
#include "../Config.h"

#include "../PDObjects/Debug_pd.h"

AmbisonicBinauralDecoder::AmbisonicBinauralDecoder(int order , int numVirtualOutputs,HrtfSize size):
AudioProcessorBase(AudioProcessorOutput),
m_ambiDecoder(new AmbisonicDecoder(order,numVirtualOutputs) ),
m_order(order),
m_numberOfVirtualOutputs(numVirtualOutputs),
m_size(size)
{

    m_tempOutput = NULL;
    m_tempBuffer = NULL;

    m_hrtfArray = new float*[2];

    m_numSamplesPerImpulse = HrtfReader::HrtfLength;

    const int sampleSize = numVirtualOutputs*m_numSamplesPerImpulse;

    //m_hrtfArray[0] = new float[sampleSize];
    //m_hrtfArray[1] = new float[sampleSize];

    m_hrtfArray[0] = (float*) aligned_alloc(16, sizeof(float)*sampleSize);
    m_hrtfArray[1] = (float*) aligned_alloc(16, sizeof(float)*sampleSize);

    FloatComputation::clearVector(m_hrtfArray[0], sampleSize);
    FloatComputation::clearVector(m_hrtfArray[1], sampleSize);


    // 72 indexes : 0 à 355

    //! 355 : va changer si l'on utilise un autre set de HRTFs
    int angle = 355/m_numberOfVirtualOutputs;
    post("angle %i",angle);

    int index = 0;
    int pos   = 0;

    int i=0;
    for (;i<m_numberOfVirtualOutputs;i++)
    {
        int ii =round( index/5.)*5;

        HrtfReader r(ii, 44100,size);

        r.getHrtfSet(m_hrtfArray[0]+pos, m_hrtfArray[1]+pos);

        pos+=r.getNumSamples();

        index+=angle ;
    }


}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

AmbisonicBinauralDecoder::~AmbisonicBinauralDecoder()
{
    //delete [] m_hrtfArray[0];
    //delete [] m_hrtfArray[1];
    aligned_free(m_hrtfArray[0]);
    aligned_free(m_hrtfArray[1]);
    delete [] m_hrtfArray;

    deleteBuffers();

    delete m_ambiDecoder;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void AmbisonicBinauralDecoder::deleteBuffers()
{
    if (m_tempOutput!=NULL)
    {
        int i = 0;
        for(;i<m_numberOfVirtualOutputs;i++)
        {
            //delete[] m_tempOutput[i];
            aligned_free(m_tempOutput[i]);
        }

        delete[] m_tempOutput;
        m_tempOutput = NULL;
    }

    if (m_tempBuffer!=NULL)
    {
        //delete [] m_tempBuffer[0];
        aligned_free(m_tempBuffer[0]);
        //delete [] m_tempBuffer[1];
        aligned_free(m_tempBuffer[1]);
        delete [] m_tempBuffer;
        m_tempBuffer = NULL;
    }



}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void AmbisonicBinauralDecoder::internalPrepare()
{
    pdAssert(getBufferSize()>400, "ERROR : buffer size for binaural encoding >400");

    m_ambiDecoder->setConfig(getBufferSize(), getSampleRate());
    m_ambiDecoder->prepare();

    deleteBuffers();

    m_tempOutput= new float*[m_numberOfVirtualOutputs];

    int i = 0;
    for(;i<m_numberOfVirtualOutputs;i++)
    {
        //m_tempOutput[i] = new float[getBufferSize()];
        m_tempOutput[i] = (float*) aligned_alloc(16, sizeof(float)*getBufferSize());
        FloatComputation::clearVector(m_tempOutput[i], getBufferSize());
    }


    m_tempBuffer = new float*[2];
    const int tempSize  = 2*getBufferSize()+4;

    //m_tempBuffer[0] = new float[tempSize];
    m_tempBuffer[0] = (float*) aligned_alloc(16, sizeof(float)*tempSize);
    //m_tempBuffer[1] = new float[tempSize];
    m_tempBuffer[1] = (float*) aligned_alloc(16, sizeof(float)*tempSize);

    FloatComputation::clearVector(m_tempBuffer[0], tempSize);
    FloatComputation::clearVector(m_tempBuffer[1], tempSize);

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

/*
inline void AmbisonicBinauralDecoder::process(float **ins, float **outs, int bufferSize)
{

//     1 : décodage du flux ambisonique sur m_numberOfVirtualOutputs.
//     Le résultat est placé dans m_tempOutput


    m_ambiDecoder->process(ins, m_tempOutput, bufferSize);

    FloatComputation::clearVector(outs[0], bufferSize);
    FloatComputation::clearVector(outs[1], bufferSize);


     //on recopie la queue du buffer precedent à la place du buffer

    FloatComputation::copyVector(m_tempBuffer[0]+bufferSize, m_tempBuffer[0], bufferSize);
    FloatComputation::copyVector(m_tempBuffer[1]+bufferSize, m_tempBuffer[1], bufferSize);


//     On efface la queue du buffer

    FloatComputation::clearVector(m_tempBuffer[0]+bufferSize, bufferSize);
    FloatComputation::clearVector(m_tempBuffer[1]+bufferSize, bufferSize);


//     2 on effectue l'encodage Binaural pour chaque haut-parleur virtuel

    int speaker = 0;
    for(;speaker<m_numberOfVirtualOutputs;speaker++)
    {

        const int hrtfSize     = HrtfReader::HrtfLength;
        const int hrtfPosition = speaker*hrtfSize;


        int i = 0;
        for (; i<bufferSize;i++) // bufferIN
        {
            const float inVal = m_tempOutput[speaker][i];

            FloatComputation::addWithMultiply(m_tempBuffer[0]+i, m_hrtfArray[0]+hrtfPosition, inVal, hrtfSize);
            FloatComputation::addWithMultiply(m_tempBuffer[1]+i, m_hrtfArray[1]+hrtfPosition, inVal, hrtfSize);

        }
        // acum de la contrib de chaque HP dans la sortie stéréo
        FloatComputation::add(m_tempBuffer[0], outs[0],bufferSize);
        FloatComputation::add(m_tempBuffer[1], outs[1],bufferSize);

    }

}
*/





