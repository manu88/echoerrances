//
//  wavPlayer~.c
//  encoder~
//
//  Created by Manuel Deneu on 31/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "obj_pd_commons.h"
#include "WavLoader.h"
#include "FloatComputation.h"

typedef struct wavPlayer
{
    // PD' stuff
    t_object x_obj;
    t_sample f;
    
    t_sample **m_ins;
    t_sample **m_outs;
    
    int m_bufferSize;
    
    t_sample *wavBuffer;
    
    int index;
    int size;
    
    
    
    
    
} t_wavPlayer;



static t_class *wavPlayer_class;


t_int *wavPlayer_perform(t_int *w)
{
    t_wavPlayer *x	= (t_wavPlayer *)(w[1]);
    
    
    FloatComputation::copyVector(x->wavBuffer + x->index, x->m_outs[0], x->m_bufferSize);
    
    x->index +=x->m_bufferSize;
    if (x->index>=x->size)
        x->index = 0;
    
    
    return (w + 2);
}



static void wavPlayer_dsp(t_wavPlayer *x, t_signal **sp, t_symbol *s)
{
    x->index = 0;
    x->m_bufferSize = sp[0]->s_n;
    
    x->m_ins[0] = sp[0]->s_vec;
    
    
    for (int i = 0; i < 2; i++)
        x->m_outs[i]= sp[i+1]->s_vec;
    
    
    
    
    dsp_add(wavPlayer_perform, 1, x);
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
static void wavPlayer_openWavFile(t_wavPlayer *x)
{
    WavLoader loader;
    if ( loader.getWavAttributes("/Users/manueldeneu/testSounds/amelie.wav") )
    {
        post("wav  succeded ");
        post("num samples : %i",loader.getNumSamples());
        post("num channels: %i",loader.getNumChannels());
        post("sample rate : %i", loader.getSampleRate());
        
        
        x->wavBuffer = new float[loader.getNumSamples()];
        x->size = loader.getNumSamples();
        loader.openAndCopyWavFile("/Users/manueldeneu/testSounds/amelie.wav", x->wavBuffer);
        
        
        
    }
    else
        post("error ");
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void *wavPlayer_new(t_symbol *s, long argc, t_atom *argv)
{
    t_wavPlayer *x = NULL;
    x = (t_wavPlayer *)pd_new(wavPlayer_class);
    if (x)
	{
        
        

        
        
        x->m_outs = new float*[2];
        
        x->m_ins = new float*[1];
        x->m_ins[0] = NULL;
        
        
        for (int i = 0; i < 2; i++)
        {
			outlet_new(&x->x_obj, &s_signal);
            x->m_outs[i] = NULL;
        }
        
        x->wavBuffer = NULL;
        wavPlayer_openWavFile(x);        
        
    }
    
    return x;
    
}

static void wavPlayer_free(t_wavPlayer *x)
{
    if (x)
    {
        

        delete[] x->m_ins[0];
        delete[] x->m_ins;
        
        if (x->wavBuffer!=NULL)
            delete [] x->wavBuffer;
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

extern "C" void wavPlayer_tilde_setup(void)
{
    wavPlayer_class = class_new(gensym("wavPlayer~"),
                               (t_newmethod)wavPlayer_new,
                               (t_method)wavPlayer_free, sizeof(t_wavPlayer),
                               CLASS_DEFAULT,
                               A_GIMME, 0);
    

    
    
    // in params
    
    
    
    class_addmethod(wavPlayer_class, (t_method)wavPlayer_dsp, gensym("dsp"), A_CANT);
    
    CLASS_MAINSIGNALIN(wavPlayer_class, t_wavPlayer, f);
    
    
}