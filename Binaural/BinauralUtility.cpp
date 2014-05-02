//
//  BinauralUtility.cpp
//  encoder~
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include <string>

#include "BinauralUtility.h"
#include "../AudioTools/AudioTools.h"


#include "../PDObjects/Debug_pd.h"



HrtfReader::HrtfReader(int angle,int sampleRate , HrtfSize size) :
m_angle(angle),
m_sampleRate(sampleRate),
m_numSamples(0),
m_size(size),
m_channels(0)

{

}

HrtfReader::~HrtfReader()
{
    closeFile();
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void HrtfReader::getHrtfSet(float* bufferL, float* bufferR)
{

    std::string url = HRTF_FILEPATH;
    url+=std::to_string(m_sampleRate);

    if (m_size == Large)
        url+="/Large/";
    else
        url+="/Small/";


    std::string urlL = url;
    std::string urlR = url;

    urlL+="left";
    urlR+="right";

    urlL+=std::to_string(m_angle);
    urlR+=std::to_string(m_angle);

    urlL+=".wav";
    urlR+=".wav";



    bool isOk = openAndCopyWavFile(urlL.c_str(), bufferL);
    pdAssert(isOk, "left ERROR :");
#ifndef __arm__
    for(int i=0; i < HrtfReader::HrtfLength; i+=4)
    {
        std::swap(bufferL[i], bufferL[i+3]);
        std::swap(bufferL[i+1], bufferL[i+2]);
    }
#endif // __arm__

    isOk = openAndCopyWavFile(urlR.c_str(),bufferR);
    pdAssert(isOk, "right ERROR");

#ifndef __arm__
    for(int i=0; i < HrtfReader::HrtfLength; i+=4)
    {
        std::swap(bufferR[i], bufferR[i+3]);
        std::swap(bufferR[i+1], bufferR[i+2]);
    }
#endif // __arm__

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

