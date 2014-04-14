//
//  Library~.c
//  audioLib
//
//  Created by Manuel Deneu on 09/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//



/*
    Librarie complète
 */
#include "obj_pd_commons.h"
#include "GrandMaster.h"



extern "C" void encoder_tilde_setup();
extern "C" void decoder_tilde_setup();
extern "C" void ambiToBinaural_tilde_setup();
extern "C" void libStat_setup();





extern "C" void Library_setup()
{

    getComputationVersion();
    encoder_tilde_setup();
    decoder_tilde_setup();
    ambiToBinaural_tilde_setup();
    libStat_setup();
    
    
}
