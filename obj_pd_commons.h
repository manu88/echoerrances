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
#include "GrandMaster.h"

static void getComputationVersion()
{
    
#ifdef USE_BASIC_COMPUTATION
    post(" BASIC COMPUTATION, it's *not* very effective...");


#elif defined __APPLE__
    post("APPLE SIMD, it's very effective...");

    
    
#elif defined __arm__
    post("ARM NEON, it's very effective...");
#endif
}


static void libStats(void)
{
    getComputationVersion();
    post("nb encoders : %i",AmbisonicUtility::getEncoderCount());
    post("nb decoders : %i",AmbisonicUtility::getDecoderCount());
    post("distance max = %i",AmbisonicEncoder::MaxDistance);
    
    post("num nodes %i", GrandMaster::getMainAudioGraph()->getNodeCount() );
}

#endif
