//
//  BinauralUtility.h
//  encoder~
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __encoder___BinauralUtility__
#define __encoder___BinauralUtility__

#include <iostream>
#include "../AudioTools/WavLoader.h"
#include "../Config.h"

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

typedef enum
{
    Small,
    Large
} HrtfSize;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

class HrtfReader : public WavLoader
{
public :

    enum {HrtfLength = HRTF_LENGTH};

    HrtfReader(int angle, int sampleRate , HrtfSize size );
    ~HrtfReader();


    void getHrtfSet(float* bufferL, float* bufferR);

private:


    int      m_angle;
    int      m_sampleRate;
    int      m_numSamples;
    HrtfSize m_size;
    int      m_channels;





};

#endif /* defined(__encoder___BinauralUtility__) */
