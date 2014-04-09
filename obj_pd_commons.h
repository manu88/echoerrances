//
//  obj_pd_commons.h
//  encoder~
//
//  Created by Manuel Deneu on 31/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef encoder__obj_pd_commons_h
#define encoder__obj_pd_commons_h

#include "m_pd.h"
#include "Debug_pd.h"
#include "Ambisonic.h"

static void getComputationVersion(const char* from)
{
    
#ifdef USE_BASIC_COMPUTATION
    post("%s uses BASIC COMPUTATION, it's *not* very effective...",from);


#elif defined __APPLE__
    post("%s uses APPLE SIMD, it's very effective...",from);

    
    
#elif defined __arm__
    post("%s uses ARM NEON, it's very effective...",from);
#endif
}


static void libStats(void)
{
    post("nb encoders : %i",AmbisonicUtility::getEncoderCount());
    post("nb decoders : %i",AmbisonicUtility::getDecoderCount());
    post("distance max = %i",AmbisonicEncoder::MaxDistance);
}

#endif
