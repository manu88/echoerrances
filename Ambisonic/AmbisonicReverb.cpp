//
//  AmbisonicReverb.cpp
//  audioLib
//
//  Created by Manuel Deneu on 03/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "AmbisonicReverb.h"
AmbisonicReverb::AmbisonicReverb(int order):
AudioProcessorBase(AudioProcessorFX),
InternalAmbisonic(order)
{
        
}

AmbisonicReverb::~AmbisonicReverb()
{
    
}


void AmbisonicReverb::internalPrepare()
{
    
}