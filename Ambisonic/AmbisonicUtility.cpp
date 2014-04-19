//
//  AmbisonicUtility.cpp
//  encoder~
//
//  Created by Manuel Deneu on 25/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "AmbisonicUtility.h"
#include "../AudioTools/AudioTools.h"
#include <stdlib.h>




int               AmbisonicUtility::s_refCount = 0;
AmbisonicUtility* AmbisonicUtility::s_instance = nullptr;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int InternalAmbisonic::getHarmonicIndex(int index)
{
    int i = 0;

    if (index == 0)
        return 0;

    else if( (index > 0) && (index < m_numberOfHarmonics) )
    {
        i = (index - 1) / 2. + 1.;

        if (index % 2 == 1)
            i *=-1;

        return i;
    }

    return 0;
}

int InternalAmbisonic::getHarmonicOrder(int index)
{
    return abs(getHarmonicIndex(index));
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

InternalAmbisonicEncoder::InternalAmbisonicEncoder(int order) :
InternalAmbisonic(order)
{
    m_encoderMatrix = new float*[m_numberOfHarmonics];

    for (int i=0; i<m_numberOfHarmonics ; i++)
        m_encoderMatrix[i] = new float[numberOfCirclePoints];

    computeEncodingMatrix();
}

InternalAmbisonicEncoder::~InternalAmbisonicEncoder()
{

    for (int i=0; i<m_numberOfHarmonics ; i++)
        delete[] m_encoderMatrix[i];

    delete[] m_encoderMatrix;
}



void InternalAmbisonicEncoder::computeEncodingMatrix()
{
    int i = 0;
    for(; i < m_numberOfHarmonics; i++)
	{
        int j = 0;
        if(getHarmonicIndex(i) >= 0)
        {
            for(/* j = 0 */; j < numberOfCirclePoints; j++)
            {
                m_encoderMatrix[i][j] = cos((float)getHarmonicOrder(i) * (((float)j / (float)numberOfCirclePoints) * (float)M_PI*2 ));
            }
        }
        else
        {
            for(/*j = 0*/; j < numberOfCirclePoints; j++)
            {
                m_encoderMatrix[i][j] = sin((float)getHarmonicOrder(i) * (((float)j / (float)numberOfCirclePoints) * (float)M_PI*2));
            }
        }
    }

}
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

InternalAmbisonicDecoder::InternalAmbisonicDecoder(int order, int numberOfOutputs):
InternalAmbisonic(order),
m_outputCount(numberOfOutputs)
{
    m_decoderMatrix = new float*[m_outputCount];

    int i=0;
    for (; i<m_outputCount ; i++)
        m_decoderMatrix[i] = new float[m_numberOfHarmonics];

    computeDecodingMatrix();

}
InternalAmbisonicDecoder::~InternalAmbisonicDecoder()
{
    for (int i=0; i<m_outputCount ; i++)
        delete[] m_decoderMatrix[i];

    delete[] m_decoderMatrix;
}

void InternalAmbisonicDecoder::computeDecodingMatrix()
{
    int i = 0;
    for (; i < m_outputCount; i++)
	{
		float angle = M_PI* 2 * ((float)i / (float)(m_outputCount));
        angle = AudioTools::radianWrap(angle);

        int j = 0;
		for (; j < m_numberOfHarmonics; j++)
		{
            int index = getHarmonicIndex(j);
            if(j == 0)
            {
                m_decoderMatrix[i][j] = 0.5 / (float)(m_order+1.);

            }
            else if(index > 0)
            {
                float value = cos(fabs((float)index) * angle) / (float)(m_order+1.);
                m_decoderMatrix[i][j] = value;

            }
			else if(index < 0)
            {
                float value = sin(fabs((float)index) * angle) / (float)(m_order+1.);
                m_decoderMatrix[i][j] = value;
            }
		}
    }

}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

AmbisonicUtility::AmbisonicUtility()
{

}

AmbisonicUtility::~AmbisonicUtility()
{

    m_encoderList.clear();
    m_encoderList.clear();

}

InternalAmbisonicEncoder* AmbisonicUtility::getEncoderForOrder(int order,bool retain)
{
    InternalAmbisonicEncoder *encoder = containsEncoder(order);

    if (encoder == nullptr)
    {
        encoder = new InternalAmbisonicEncoder(order);
        m_encoderList.push_back(encoder);
    }
    if (retain)
        encoder->retain();

    return encoder;
}

InternalAmbisonicDecoder* AmbisonicUtility::getDecoderForOrder(int order, int numberOfOutputs,bool retain)
{
    InternalAmbisonicDecoder *decoder = containsDecoder(order, numberOfOutputs);

    if (decoder==nullptr)
    {
        decoder = new InternalAmbisonicDecoder(order,numberOfOutputs);
        m_decoderList.push_back(decoder);
    }
    if (retain)
        decoder->retain();


    return decoder;
}


InternalAmbisonicEncoder* AmbisonicUtility::containsEncoder(int order)
{

    for(std::list<InternalAmbisonicEncoder*>::iterator iter = m_encoderList.begin();iter != m_encoderList.end(); iter++)
    {
        if ((*iter)->getOrder()==order)
            return *iter;
    }
    return nullptr;
}

InternalAmbisonicDecoder* AmbisonicUtility::containsDecoder(int order , int numOuts)
{
    for(std::list<InternalAmbisonicDecoder*>::iterator iter = m_decoderList.begin();iter != m_decoderList.end(); iter++)
    {
        if ( ((*iter)->getOrder()==order) && ( (*iter)->getNumberOfOutputs()==numOuts))
            return *iter;
    }
    return nullptr;
}


void AmbisonicUtility::inspectEncoderList()
{
    for(std::list<InternalAmbisonicEncoder*>::iterator iter = m_encoderList.begin();iter != m_encoderList.end(); iter++)
    {
        if ((*iter)->getRefCount() == 0)
            m_encoderList.remove( (*iter));

    }

}

void AmbisonicUtility::inspectDecoderList()
{
    for(std::list<InternalAmbisonicDecoder*>::iterator iter = m_decoderList.begin();iter != m_decoderList.end(); iter++)
    {

        if ((*iter)->getRefCount() == 0)
            m_decoderList.remove( (*iter));
    }

}



