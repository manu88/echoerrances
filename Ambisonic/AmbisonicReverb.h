//
//  AmbisonicReverb.h
//  audioLib
//
//  Created by Manuel Deneu on 03/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __audioLib__AmbisonicReverb__
#define __audioLib__AmbisonicReverb__

#include <iostream>

#include "AmbisonicUtility.h"
#include "AudioInternals.h"
#include "FloatComputation.h"

#include "Debug_pd.h"

class AmbisonicReverb : public InternalAmbisonic, public AudioProcessorBase
{
public:
    AmbisonicReverb(int order);
    ~AmbisonicReverb();
    virtual inline void process(float **ins, float **outs, int bufferSize)
    {

        int i = 0;
        for (; i<getHarmonicNumber();i++)
        {
            FloatComputation::copyVector(ins[i], outs[i], bufferSize);
        }
    }
    
    
    virtual void internalPrepare();
    
private:
    
    


};
#endif /* defined(__audioLib__AmbisonicReverb__) */
