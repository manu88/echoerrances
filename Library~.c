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
#include "m_pd.h"



extern void encoder_tilde_setup();
extern void decoder_tilde_setup();
extern void ambiToBinaural_tilde_setup();





void Library_setup()
{
    post("library setup");
    encoder_tilde_setup();
    decoder_tilde_setup();
    ambiToBinaural_tilde_setup();
    
    
}


void test_setup()
{
    post("bla");
}