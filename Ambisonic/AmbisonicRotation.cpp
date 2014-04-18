//
//  AmbisonicRotation.cpp
//  audioLib
//
//  Created by Manuel Deneu on 17/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "AmbisonicRotation.h"



AmbisonicRotation::AmbisonicRotation(int order):
InternalAmbisonic(order),
AudioProcessorBase(AudioProcessorFX),
m_angle(0.),
m_cosLookUp(new float[numberOfCirclePoints]),
m_sinLookUp(new float[numberOfCirclePoints]),
m_harmonicSin(new float[m_order]),
m_harmonicCos(new float[m_order])
{
    int i = 0;
    for (; i < numberOfCirclePoints; i++)
	{
		m_cosLookUp[i] = cos((float)i * 2*M_PI / numberOfCirclePoints);
		m_sinLookUp[i] = sin((float)i * 2*M_PI / numberOfCirclePoints);
	}
}

AmbisonicRotation::~AmbisonicRotation()
{
    delete [] m_cosLookUp;
    delete [] m_sinLookUp;
    delete [] m_harmonicSin;
    delete [] m_harmonicCos;
    
    setAngle(0.);
}




void AmbisonicRotation::setAngle(float newAngle)
{
    if (newAngle!=m_angle)
    {
        int tmpAngle;
        
        if (newAngle < 0)
            newAngle += ( -floor(newAngle/2*M_PI)) * 2*M_PI;

        double tmpTheta = newAngle;
        m_angle = fmod(newAngle + 2*M_PI, 2*M_PI);

        int i = 0;
        for(; i < m_order; i++)
        {
            double angleFactor = tmpTheta*numberOfCirclePoints/(2*M_PI);
            
            tmpAngle = (long)(angleFactor)%(numberOfCirclePoints-1);
            m_harmonicSin[i] = m_sinLookUp[tmpAngle];
            m_harmonicCos[i] = m_cosLookUp[tmpAngle];
            
            tmpTheta += newAngle;
        }
        m_angle = newAngle;
    }
    
    else //ne devrais jamais arriver...
        pdAssert(false, "AmbisonicRotation::setAngle");
    
}

void AmbisonicRotation::internalPrepare()
{
    
}