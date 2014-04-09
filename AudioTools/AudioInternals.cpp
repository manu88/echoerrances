//
//  AudioInternals.cpp
//  audioLib
//
//  Created by Manuel Deneu on 17/03/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "AudioInternals.h"


AudioProcessorBase::AudioProcessorBase():
m_bufferSize(64),
m_sampleRate(44.100)
{
    
}

AudioProcessorBase::~AudioProcessorBase()
{
    
}

void AudioProcessorBase::setConfig(int bufferSize, double sampleRate)
{
    setSampleRate(sampleRate);
    setBufferSize(bufferSize);
}

 void AudioProcessorBase::prepare()
{
}