
//
//  ambiToBinaural~.c
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "m_pd.h"
#include "Binaural.h"
#include "obj_pd_commons.h"
#include "GrandMaster.h"

typedef struct ambiToBinaural
{
    // PD' stuff
    t_object x_obj;
    t_sample f;

    AmbisonicBinauralDecoder *m_proc;    
    int m_nodeId;


    
    t_sample **m_ins;
    t_sample **m_outs;
    


    
} t_ambiToBinaural;

static t_class *ambiToBinaural_class;


t_int *ambiToBinaural_perform(t_int *w)
{
    t_ambiToBinaural *x	= (t_ambiToBinaural *)(w[1]);
    
    
    x->m_proc->process(x->m_ins, x->m_outs, x->m_proc->getBufferSize());
    
    return (w + 2);
}



static void ambiToBinaural_dsp(t_ambiToBinaural *x, t_signal **sp, t_symbol *s)
{

    x->m_proc->setConfig(sp[0]->s_n, sp[0]->s_sr);
    x->m_proc->prepare();
    

    int i=0;
    for (; i<x->m_proc->getNumOfHarmonics();i++)
        x->m_ins[i] = sp[i]->s_vec;
    
    

    x->m_outs[0]= sp[i]->s_vec;
    x->m_outs[1]= sp[i+1]->s_vec;

    

    dsp_add(ambiToBinaural_perform, 1, x);
}

static void ambiToBinaural_bang(t_ambiToBinaural *x)
{
    post("encoder node ID = %i",x->m_nodeId);
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void *ambiToBinaural_new(t_symbol *s, long argc, t_atom *argv)
{
    t_ambiToBinaural *x = NULL;
    x = (t_ambiToBinaural *)pd_new(ambiToBinaural_class);
    if (x)
	{
        int order = 0;
        int numVirtualSpeakers = 0;
        if (argc >= 2)
        {
            order   = atom_getint (argv);
            numVirtualSpeakers = atom_getint(argv+1);
            
            
        }
        
        if (order == 0)
            order = 4;
        
        if (numVirtualSpeakers == 0)
            numVirtualSpeakers = (order+1)*2;
        
        post("order %i HPs %i",order,numVirtualSpeakers);
        
        
        x->m_proc = new AmbisonicBinauralDecoder(order,numVirtualSpeakers , Large);
        
        
        x->m_outs = new float*[2];
        
        x->m_ins = new float*[x->m_proc->getNumOfHarmonics()];
        x->m_ins[0] = NULL;
        
        int i=1;
        
        for (/* i=1 */;i<x->m_proc->getNumOfHarmonics();i++)
        {
            //signalinlet_new(&x->x_obj, x->f);
            inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
            x->m_ins[i]= NULL;
            
        }

        

        outlet_new(&x->x_obj, &s_signal);
        outlet_new(&x->x_obj, &s_signal);        
        x->m_outs[0] = NULL;
        x->m_outs[1] = NULL;

        x->m_nodeId = GrandMaster::registerForAudioProcessor(x->m_proc);
        post("ref count = %i",GrandMaster::getRefCount());        
        
    }

    return x;

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void ambiToBinaural_free(t_ambiToBinaural *x)
{
    if (x)
    {
        GrandMaster::getMainAudioGraph()->removeNode(x->m_nodeId);
        GrandMaster::release();
       
        delete[] x->m_outs;
        

        
        delete[] x->m_ins;
        
        delete x->m_proc;
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

extern "C" void ambiToBinaural_tilde_setup()
{
    ambiToBinaural_class = class_new(gensym("ambiToBinaural~"),
                              (t_newmethod)ambiToBinaural_new,
                              (t_method)ambiToBinaural_free, sizeof(t_ambiToBinaural),
                              CLASS_DEFAULT,
                              A_GIMME, 0);


    
    
    class_addmethod(ambiToBinaural_class, (t_method)ambiToBinaural_dsp, gensym("dsp"), A_CANT);
    class_addbang(ambiToBinaural_class, ambiToBinaural_bang);    
    CLASS_MAINSIGNALIN(ambiToBinaural_class, t_ambiToBinaural, f);
    
    
}


