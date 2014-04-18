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


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Decoder ambisonic->binaural
    params:
        - ordre
        - numVirtualOutputs : nb de hp (virtuels) sur lesquels décoder le champ ambisonique
 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class AmbisonicBinauralDecoder : public AudioProcessorBase
{
public:
    AmbisonicBinauralDecoder(int order , int numVirtualOutputs , HrtfSize size );
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
    
    
    /*virtual*/ inline void process(float **ins, float **outs, int bufferSize)
    {
        const float att = 1. /m_numberOfVirtualOutputs;
        
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
            FloatComputation::addWithMultiply( outs[0] , m_tempBuffer[0] , att , bufferSize);
            FloatComputation::addWithMultiply( outs[1] , m_tempBuffer[1] , att , bufferSize);
            

            
            
        }
        /*
        for (int i = 0; i<bufferSize;i++) 
        {
            pdAssert( ( (outs[0][i] <=1.) && (outs[0][i] >=-1. ) ) , "val out of [-1;1] in ambiBinau 0");
            pdAssert( ( (outs[1][i] <=1.) && (outs[1][i] >=-1. ) ) , "val out of [-1;1] in ambiBinau 1");
        }
        */
    }

    
    
private:
    virtual void   internalPrepare();
    
    AmbisonicDecoder* m_ambiDecoder;

    void deleteBuffers();
    
    float**  m_tempOutput; // buffer temp. après décodage ambi
    float**  m_tempBuffer; // buffer temp. pour binaural (2* buffersize pour stocker la queue de la convo)
    
    int      m_order;
    int      m_numberOfVirtualOutputs;
    float**  m_hrtfArray;
    int      m_numSamplesPerImpulse;
    
    HrtfSize m_size;
    

};

#endif /* defined(__encoder___Binaural__) */
