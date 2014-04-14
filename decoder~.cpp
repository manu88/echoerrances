//
//  2Dencoder.c
//  2Dencoder
//
//  Created by Manuel Deneu on 25/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "m_pd.h"
#include "Ambisonic.h"
#include "FloatComputation.h"
#include "AudioTools.h"
#include "obj_pd_commons.h"
#include "GrandMaster.h"

typedef struct decoder
{
    // PD' stuff
    t_object x_obj;
    t_sample f;
    
    int m_nodeId;
    
    t_sample **m_ins;
    t_sample **m_outs;
    
    // ambisonic stuffs
    

    
    AmbisonicDecoder* m_decoder;

    
    
    
    
} t_decoder;

static t_class *decoder_class;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

t_int *decoder_perform(t_int *w)
{
    t_decoder *x	= (t_decoder *)(w[1]);

    
    x->m_decoder->process(x->m_ins, x->m_outs, x->m_decoder->getBufferSize() );
    
    return (w + 2);
}



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */


static void decoder_dsp(t_decoder *x, t_signal **sp, t_symbol *s)
{

    x->m_decoder->setConfig(sp[0]->s_n, sp[0]->s_sr);

    x->m_decoder->prepare();
    
    int i =0;
    for (;i < x->m_decoder->getHarmonicNumber();i++)
        x->m_ins[i] = sp[i]->s_vec;
    

    for (i = 0; i < x->m_decoder->getNumOuts(); i++)
        x->m_outs[i]= sp[i+x->m_decoder->getHarmonicNumber()]->s_vec;


    
    
    dsp_add(decoder_perform, 1, x);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void decoder_getStats(t_decoder *x )
{
    libStats();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void *decoder_new(t_symbol *s, long argc, t_atom *argv)
{
    t_decoder *x = NULL;
    x = (t_decoder *)pd_new(decoder_class);
    if (x)
	{
        int order = 0;
        int numOuts = 0;
        if (argc>=2)
        {
            order   = atom_getint (argv);
            numOuts = atom_getint(argv+1);
        }
        
        if (order==0)
            order = 4;
        
        if (numOuts==0)
            numOuts = 8;
        
        
        x->m_decoder = new AmbisonicDecoder(order,numOuts);

        

        
        x->m_ins = new float*[x->m_decoder->getHarmonicNumber()];
        x->m_outs = new float*[x->m_decoder->getNumOuts()];
        
        for (int i = 0; i < x->m_decoder->getNumOuts(); i++)
        {
			outlet_new(&x->x_obj, &s_signal);
            x->m_outs[i] = NULL;
        }
        
        x->m_ins[0] = NULL;        
        for (int i= 1;i<x->m_decoder->getHarmonicNumber();i++)
        {
            signalinlet_new(&x->x_obj, x->f);
            x->m_ins[i] = NULL;
        }
        
        GrandMaster::retain();
        post("ref count = %i",GrandMaster::getRefCount());
        x->m_nodeId = GrandMaster::getMainAudioGraph()->addNode(x->m_decoder);
    
    }
    

    post("order %i", x->m_decoder->getOrder() );
    post("ins %i",x->m_decoder->getHarmonicNumber() );
    post("outs %i",x->m_decoder->getNumOuts() );
    
    
    return x;
    
    
    
}

static void decoder_free(t_decoder *x)
{
    post("delete");
    if (x)
    {
        GrandMaster::getMainAudioGraph()->removeNode(x->m_nodeId);        
        GrandMaster::release();

        
        delete[] x->m_ins;
        delete[] x->m_outs;
        
        
        delete x->m_decoder;
        
        
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

extern "C" void decoder_tilde_setup()
{
    decoder_class = class_new(gensym("decoder~"),
                                     (t_newmethod)decoder_new,
                                     (t_method)decoder_free, sizeof(t_decoder),
                                     CLASS_DEFAULT,
                                     A_GIMME, 0);
    
    
    // in params
    
    
    
    class_addmethod(decoder_class, (t_method)decoder_dsp, gensym("dsp"), A_CANT);
    class_addmethod(decoder_class, (t_method)decoder_getStats, gensym("stats"), A_NULL);
    
    CLASS_MAINSIGNALIN(decoder_class, t_decoder, f);
    
    
}
