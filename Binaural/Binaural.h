//
//  Binaural.h
//  encoder~
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __encoder___Binaural__
#define __encoder___Binaural__

#include "Ambisonic.h"
#include "BinauralUtility.h"
#include "FloatComputation.h"
#include "AudioInternals.h"

/*
    Decoder ambisonic->binaural
    params:
        - ordre
        - numVirtualOutputs : nb de hp (virtuels) sur lesquels décoder le champ ambisonique
 */
class AmbisonicBinauralDecoder : public AudioProcessorBase
{
public:
    AmbisonicBinauralDecoder(int order , int numVirtualOutputs);
    ~AmbisonicBinauralDecoder();

    
    int getOrder() const
    {
        return m_order;
    }
    
    int getNumOfHarmonics() const
    {
        return m_order*2 + 1;
    }
    
    int getNumOfVirtualOutputs() const
    {
        return m_numberOfVirtualOutputs;
    }
    
    float* getHrtfArrayForSpeaker(int numSpeaker, int channel, int offset = 0)
    {
        return (m_hrtfArray[channel]+(m_numSamplesPerImpulse*numSpeaker)+offset );
    }
    
    
    virtual void   prepare();
    
    inline void process(float **ins, float **outs, int bufferSize)
    {
        /*
         1 : décodage du flux ambisonique sur m_numberOfVirtualOutputs. 
             Le résultat est placé dans m_tempOutput
         */
        
        m_ambiDecoder->process(ins, m_tempOutput, bufferSize);
        

        
        /* 
         on recopie la queue du buffer precedent à la place du buffer
         */
        
        FloatComputation::copyVector(m_tempBuffer[0]+bufferSize, m_tempBuffer[0], bufferSize);
        FloatComputation::copyVector(m_tempBuffer[1]+bufferSize, m_tempBuffer[1], bufferSize);
        
        
        FloatComputation::clearVector(m_tempBuffer[0]+bufferSize-1, bufferSize);
        FloatComputation::clearVector(m_tempBuffer[1]+bufferSize-1, bufferSize);
        
        /*
         2 on effectue l'encodage Binaural pour chaque haut-parleur virtuel
         */
        int speaker = 0;
        
        for(;speaker<m_numberOfVirtualOutputs;speaker++)
        {
            const int hrtfSize     = HrtfReader::HrtfLength;
            const int hrtfPosition = speaker*hrtfSize;//m_testAngle; //getIndexForAzimuth(m_azimuth);

            
            
            for (int i = bufferSize; i--;) // bufferIN
            {
                const float inVal = ins[speaker][i];
                
                FloatComputation::addWithMultiply(m_tempBuffer[0]+i, m_hrtfArray[0]+hrtfPosition, inVal, hrtfSize);
                FloatComputation::addWithMultiply(m_tempBuffer[1]+i, m_hrtfArray[1]+hrtfPosition, inVal, hrtfSize);
                
            }
            
            FloatComputation::copyVector(m_tempBuffer[0], outs[0], bufferSize);
            FloatComputation::copyVector(m_tempBuffer[1], outs[1], bufferSize);
        }

    }
    
    
private:
    
    AmbisonicDecoder* m_ambiDecoder;
    
    float** m_tempOutput;
    
    int     m_order;
    int     m_numberOfVirtualOutputs;
    float** m_hrtfArray;
    int     m_numSamplesPerImpulse;
    
    
    float** m_tempBuffer;
};

#endif /* defined(__encoder___Binaural__) */
