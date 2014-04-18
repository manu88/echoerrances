//
//  AmbisonicRotation.h
//  audioLib
//
//  Created by Manuel Deneu on 17/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __audioLib__AmbisonicRotation__
#define __audioLib__AmbisonicRotation__

#include <iostream>
#include "AmbisonicUtility.h"
#include "AudioInternals.h"
#include "FloatComputation.h"

#include "Debug_pd.h"

class AmbisonicRotation : public InternalAmbisonic, public AudioProcessorBase
{
public:
    AmbisonicRotation(int order);
    ~AmbisonicRotation();
    
    void  setAngle(float newAngle);

    float getAngle() const
    {
        return m_angle;
    }
    
    
    inline void process(float **ins, float **outs, int bufferSize)
    {
        FloatComputation::copyVector(ins[0],outs[0], bufferSize);

        int i = 1   ;
        for(; i < m_order; i++)
		{
            int j = 0;
            
			for(; j < bufferSize; j++)
			{
				outs[2*i]  [j]	= m_harmonicCos[i-1] * ins[2*i][j] - m_harmonicSin[i-1] * ins[2*i-1][j];
				outs[2*i-1][j]	= m_harmonicSin[i-1] * ins[2*i][j] + m_harmonicCos[i-1] * ins[2*i-1][j];
			}
		}
    }
    
private:

    
    virtual void   internalPrepare();
    
    float       m_angle;
    float*		m_cosLookUp;
	float*		m_sinLookUp;
	float*		m_harmonicSin;
	float*		m_harmonicCos;
};


#endif /* defined(__audioLib__AmbisonicRotation__) */
