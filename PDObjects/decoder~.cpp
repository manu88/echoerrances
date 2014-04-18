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

    AmbisonicDecoder* m_proc;    
    int m_nodeId;
    
    t_sample **m_ins;
    t_sample **m_outs;
    
    // ambisonic stuffs
    

    


    
    
    
    
} t_decoder;

static t_class *decoder_class;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

t_int *decoder_perform(t_int *w)
{
    t_decoder *x	= (t_decoder *)(w[1]);

    
    x->m_proc->process(x->m_ins, x->m_outs, x->m_proc->getBufferSize() );
    
    return (w + 2);
}



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */


static void decoder_dsp(t_decoder *x, t_signal **sp, t_symbol *s)
{

    x->m_proc->setConfig(sp[0]->s_n, sp[0]->s_sr);

    x->m_proc->prepare();
    
    int i =0;
    for (;i < x->m_proc->getHarmonicNumber();i++)
        x->m_ins[i] = sp[i]->s_vec;
    

    for (i = 0; i < x->m_proc->getNumOuts(); i++)
        x->m_outs[i]= sp[i+x->m_proc->getHarmonicNumber()]->s_vec;


    
    
    dsp_add(decoder_perform, 1, x);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void decoder_getStats(t_decoder *x )
{
    libStats();
}


static void decoder_bang(t_decoder *x)
{
    post("decoder node ID = %i",x->m_nodeId);
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
        
        
        x->m_proc = new AmbisonicDecoder(order,numOuts);

        

        
        x->m_ins = new float*[x->m_proc->getHarmonicNumber()];
        x->m_outs = new float*[x->m_proc->getNumOuts()];
        
        for (int i = 0; i < x->m_proc->getNumOuts(); i++)
        {
			outlet_new(&x->x_obj, &s_signal);
            x->m_outs[i] = NULL;
        }
        
        x->m_ins[0] = NULL;        
        for (int i= 1;i<x->m_proc->getHarmonicNumber();i++)
        {
            signalinlet_new(&x->x_obj, x->f);
            x->m_ins[i] = NULL;
        }
        
        x->m_nodeId = GrandMaster::registerForAudioProcessor(x->m_proc);
        post("ref count = %i",GrandMaster::getRefCount());

    
    }
    

    post("order %i", x->m_proc->getOrder() );
    post("ins %i",x->m_proc->getHarmonicNumber() );
    post("outs %i",x->m_proc->getNumOuts() );
    
    
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
        
        
        delete x->m_proc;
        
        
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
    class_addbang(decoder_class, decoder_bang);
    
    CLASS_MAINSIGNALIN(decoder_class, t_decoder, f);
    
    
}
