//
//  2Dencoder.c
//  2Dencoder
//
//  Created by Manuel Deneu on 25/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//
#include "obj_pd_commons.h"

#include "Ambisonic.h"
#include "FloatComputation.h"
#include "AudioTools.h"
#include "AudioInternals.h"
#include "GrandMaster.h"



typedef struct encoder
{
    // PD' stuff
    t_object x_obj;
    t_sample f;
    
    int m_nodeId;
    
    t_sample *m_in;
    t_sample **m_outs;
    
    float*  m_harmonics;
    
    AmbisonicEncoder *m_encoder;
    
} t_encoder;

static t_class *encoder_class;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

t_int *encoder_perform(t_int *w)
{
    t_encoder *x	= (t_encoder *)(w[1]);

    float** ins = &x->m_in;
    
        x->m_encoder->process(ins, x->m_outs, x->m_encoder->getBufferSize());

    return (w + 2);
}



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */


static void encoder_dsp(t_encoder *x, t_signal **sp, t_symbol *s)
{
    x->m_encoder->setConfig(sp[0]->s_n, sp[0]->s_sr);

    x->m_encoder->prepare();
    
    const int outs = x->m_encoder->getHarmonicNumber();
    
    x->m_in = sp[0]->s_vec;
    
    int i=0;
    for (i = 0; i < outs; i++)
//    for (i = outs-1; i >= 0; i--)
    {
        x->m_outs[i]= sp[i+1]->s_vec;
    }
    
    
    dsp_add(encoder_perform, 1, x);
}


static void encoder_getStats(t_encoder *x )
{
    libStats();
}

static void encoder_angle(t_encoder *x , t_floatarg angle)
{    
    x->m_encoder->setAngle(angle);

}
static void encoder_distance(t_encoder *x , t_floatarg dist)
{
    x->m_encoder->setDistance(dist);
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void *encoder_new(t_symbol *s, long argc, t_atom *argv)
{
    t_encoder *x = NULL;
    x = (t_encoder *)pd_new(encoder_class);
    if (x)
	{
        int order = 0;
        
        if (argc>=1)
            order = atom_getint (argv);
        
        if (order==0)
            order = 4;
        
        x->m_encoder = new AmbisonicEncoder(order);

        

        int outs = 2*order +1;


        x->m_in = new float;
        x->m_outs = new float*[outs];
        
        for (int i = 0; i < outs; i++)
        {
			outlet_new(&x->x_obj, &s_signal);
            x->m_outs[i] = NULL;
        }
        
        
        x->m_harmonics = new float[numberOfCirclePoints];
        
        
        GrandMaster::retain();
        post("ref count = %i",GrandMaster::getRefCount());
        
        x->m_nodeId = GrandMaster::getMainAudioGraph()->addNode(x->m_encoder);
        
    }
        
    return x;
    
    
    
}

static void encoder_free(t_encoder *x)
{
    if (x)
    {
        GrandMaster::getMainAudioGraph()->removeNode(x->m_nodeId);        
        GrandMaster::release();
        

        
        delete x->m_in;
        delete[] x->m_outs;
        
        delete[] x->m_harmonics;
        
        delete x->m_encoder;
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

extern "C" void encoder_tilde_setup()
{

    
    encoder_class = class_new(gensym("encoder~"),
                                     (t_newmethod)encoder_new,
                                     (t_method)encoder_free, sizeof(t_encoder),
                                     CLASS_DEFAULT,
                                     A_GIMME, 0);
    
    
    
    // in params
    
    
    
    class_addmethod(encoder_class, (t_method)encoder_dsp, gensym("dsp"), A_CANT);
    class_addmethod(encoder_class, (t_method)encoder_angle, gensym("angle"), A_FLOAT);
    class_addmethod(encoder_class, (t_method)encoder_distance, gensym("dist"), A_FLOAT);
    
    
    class_addmethod(encoder_class, (t_method)encoder_getStats, gensym("stats"), A_NULL);

    CLASS_MAINSIGNALIN(encoder_class, t_encoder, f);
    
    
}
