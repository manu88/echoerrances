//
//  AmbisonicReverb~.c
//  audioLib
//
//  Created by Manuel Deneu on 03/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include <stdio.h>
#include "obj_pd_commons.h"

#include "Ambisonic.h"
#include "FloatComputation.h"
#include "AudioTools.h"

typedef struct ambisonicReverb
{
    // PD' stuff
    t_object x_obj;
    t_sample f;
  
// a apronfondir...
//    t_inlet  **m_inlets;
//    t_outlet **m_outlets;
    
    t_sample **m_ins;
    t_sample **m_outs;
    
    AmbisonicReverb* m_reverb;

    
} t_ambisonicReverb;

static t_class *ambisonicReverb_class;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

t_int *ambisonicReverb_perform(t_int *w)
{
    t_ambisonicReverb *x	= (t_ambisonicReverb *)(w[1]);
    
    x->m_reverb->process(x->m_ins, x->m_outs, x->m_reverb->getBufferSize());
    
    return (w + 2);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void ambisonicReverb_dsp(t_ambisonicReverb *x, t_signal **sp, t_symbol *s)
{
    x->m_reverb->setConfig(sp[0]->s_n, sp[0]->s_sr);
    
    x->m_reverb->prepare();
    
    const int inCount /* = outCount*/ = x->m_reverb->getHarmonicNumber();

    int i =0;
    for (;i < inCount;i++)
    {
        x->m_ins[i] = sp[i]->s_vec;
    }
    
    for (i = 0; i < inCount; i++)
    {
        x->m_outs[i]= sp[i+inCount]->s_vec;
    }
    
    dsp_add(ambisonicReverb_perform, 1, x);
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void* ambisonicReverb_new(t_symbol *s, long argc, t_atom *argv)
{
    
    t_ambisonicReverb *x = NULL;
    x = (t_ambisonicReverb *)pd_new(ambisonicReverb_class);
    if (x)
	{
        int order = 0;
        
        if (argc>=1)
            order = atom_getint (argv);
        
        if (order==0)
            order = 4;
        
        x->m_reverb = new AmbisonicReverb(order);
        
        x->m_ins  = new float*[x->m_reverb->getHarmonicNumber()];
        x->m_outs = new float*[x->m_reverb->getHarmonicNumber()];
        
        
        
        
        x->m_ins[0] = NULL;
        
        int i = 1;
        for (;i<x->m_reverb->getHarmonicNumber();i++)
        {
//            signalinlet_new(&x->x_obj, x->f);
            inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);  
            x->m_ins[i] = NULL;
        }
        for (i = 0; i < x->m_reverb->getHarmonicNumber(); i++)
        {
			outlet_new(&x->x_obj, &s_signal);
            x->m_outs[i] = NULL;
        }
        
        
        

    }
    
    return x;

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void ambisonicReverb_free(t_ambisonicReverb *x)
{
    if (x)
    {

        delete[] x->m_ins;
        delete[] x->m_outs;
        delete x->m_reverb;
    }
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

extern "C" void ambisonicReverb_tilde_setup(void)
{
    ambisonicReverb_class = class_new(gensym("ambisonicReverb~"),
                              (t_newmethod)ambisonicReverb_new,
                              (t_method)ambisonicReverb_free, sizeof(t_ambisonicReverb),
                              CLASS_DEFAULT,
                              A_GIMME, 0);
    
    getComputationVersion("ambisonicReverb");
    
    // in params
    
    
    
    class_addmethod(ambisonicReverb_class, (t_method)ambisonicReverb_dsp, gensym("dsp"), A_CANT);

    

    
    CLASS_MAINSIGNALIN(ambisonicReverb_class, t_ambisonicReverb, f);
    
    
}