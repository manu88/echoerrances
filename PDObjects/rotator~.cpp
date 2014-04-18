//
//  rotator.cpp
//  audioLib
//
//  Created by Manuel Deneu on 17/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//


#include <stdio.h>
#include "obj_pd_commons.h"

#include "Ambisonic.h"
#include "FloatComputation.h"
#include "AudioTools.h"
#include "GrandMaster.h"

typedef struct ambisonicRotator
{
    // PD' stuff
    t_object x_obj;
    t_sample f;
    
    
    t_sample **m_ins;
    t_sample **m_outs;
    
    AmbisonicRotation *m_proc;
    
    
} t_ambisonicRotator;

static t_class *ambisonicRotator_class;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

t_int *ambisonicRotator_perform(t_int *w)
{
    t_ambisonicRotator *x	= (t_ambisonicRotator *)(w[1]);
    
    x->m_proc->process(x->m_ins, x->m_outs, x->m_proc->getBufferSize());
    
    return (w + 2);
}



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void ambisonicRotator_dsp(t_ambisonicRotator *x, t_signal **sp, t_symbol *s)
{
    x->m_proc->setConfig(sp[0]->s_n, sp[0]->s_sr);
    
    const int inCount /* = outCount*/ = x->m_proc->getHarmonicNumber();
    
    int i =0;
    for (;i < inCount;i++)
    {
        x->m_ins[i] = sp[i]->s_vec;
    }
    
    for (i = 0; i < inCount; i++)
    {
        x->m_outs[i]= sp[i+inCount]->s_vec;
    }
    
    dsp_add(ambisonicRotator_perform, 1, x);

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void ambisonicRotator_angle(t_ambisonicRotator *x, t_floatarg angle)
{
    x->m_proc->setAngle(angle);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void ambisonicRotator_free(t_ambisonicRotator *x)
{
    if (x)
    {
        delete[] x->m_ins;
        delete[] x->m_outs;
        
        delete x->m_proc;
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void* ambisonicRotator_new(t_symbol *s, long argc, t_atom *argv)
{
    
    ambisonicRotator *x = NULL;
    x = (t_ambisonicRotator *)pd_new(ambisonicRotator_class);
    if (x)
	{
        int order = 0;
        
        if (argc>=1)
            order = atom_getint (argv);
        
        if (order==0)
            order = 4;
        
        
        x->m_proc = new AmbisonicRotation(order);
        
        x->m_ins  = new float*[x->m_proc->getHarmonicNumber()];
        x->m_outs = new float*[x->m_proc->getHarmonicNumber()];
        
        
        
        
        x->m_ins[0] = NULL;
        
        int i = 1;
        for (;i<x->m_proc->getHarmonicNumber();i++)
        {

            inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
            x->m_ins[i] = NULL;
        }
        for (i = 0; i < x->m_proc->getHarmonicNumber(); i++)
        {
			outlet_new(&x->x_obj, &s_signal);
            x->m_outs[i] = NULL;
        }
    }
    
    return x;
}
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

extern "C" void rotator_tilde_setup(void)
{
    ambisonicRotator_class = class_new(gensym("rotator~"),
                                      (t_newmethod)ambisonicRotator_new,
                                      (t_method)ambisonicRotator_free, sizeof(t_ambisonicRotator),
                                      CLASS_DEFAULT,
                                      A_GIMME, 0);
    
    
    // in params
    
    
    class_addmethod(ambisonicRotator_class, (t_method)ambisonicRotator_angle, gensym("angle"), A_FLOAT);
    class_addmethod(ambisonicRotator_class, (t_method)ambisonicRotator_dsp, gensym("dsp"), A_CANT);
    
    
    
    
    CLASS_MAINSIGNALIN(ambisonicRotator_class, t_ambisonicRotator, f);
    
    
}