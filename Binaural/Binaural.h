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
    
    virtual inline void process(float **ins, float **outs, int bufferSize);
    
    
private:
    
    AmbisonicDecoder* m_ambiDecoder;
    
    float** m_tempOutput; // buffer temp après décodage ambi
    
    int     m_order;
    int     m_numberOfVirtualOutputs;
    float** m_hrtfArray;
    int     m_numSamplesPerImpulse;
    
    
    float** m_tempBuffer;
};

#endif /* defined(__encoder___Binaural__) */
