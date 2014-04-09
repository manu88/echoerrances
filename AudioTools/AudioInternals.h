//
//  AudioInternals.h
//  audioLib
//
//  Created by Manuel Deneu on 17/03/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __audioLib__AudioInternals__
#define __audioLib__AudioInternals__

/*
 
 Base class for audio processors
 
 */
class AudioProcessorBase
{
public:

    int getBufferSize() const
    {
        return m_bufferSize;
    }
    
    void setBufferSize(int newBufferSize)
    {
        m_bufferSize = newBufferSize;
    }
    
    double getSampleRate() const
    {
        return m_sampleRate;
    }
    
    void setSampleRate(double newSampleRate)
    {
        m_sampleRate = newSampleRate;
    }
    
    void setConfig(int bufferSize, double sampleRate);
    
    
    virtual inline void process(float **ins, float **outs, int bufferSize) = 0;

    virtual void   prepare() = 0;
    
protected:
    AudioProcessorBase();
    ~AudioProcessorBase();
    
    int m_bufferSize;
    double m_sampleRate;
   
};





#endif /* defined(__audioLib__AudioInternals__) */
