//
//  BinauralUtility.cpp
//  encoder~
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include <string>

#include "BinauralUtility.h"
#include "AudioTools.h"


#include "Debug_pd.h"



HrtfReader::HrtfReader(int angle,int sampleRate , HrtfSize size) :
m_angle(angle),
m_sampleRate(sampleRate),
m_numSamples(0),
m_size(size),
m_channels(0),
m_currentFile(NULL)
{
    
}

HrtfReader::~HrtfReader()
{
    closeFile();
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void HrtfReader::getHrtfSet(float* bufferL, float* bufferR)
{
    
    std::string url = "/Users/manueldeneu/HrtfDatabase/";
    url+=std::to_string(m_sampleRate);
    
    if (m_size==Large)
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
    
    post("%s",urlL.c_str());
    post("%s",urlR.c_str());
    

    bool isOk = openAndCopyWavFile(urlL.c_str(), bufferL);
    pdAssert(isOk, "left ERROR :");

    isOk = openAndCopyWavFile(urlR.c_str(),bufferR);
    
    pdAssert(isOk, "right ERROR");

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

