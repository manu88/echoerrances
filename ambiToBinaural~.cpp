
//
//  ambiToBinaural~.c
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "m_pd.h"
#include "BinauralUtility.h"
#include "Binaural.h"
#include "FloatComputation.h"
#include "obj_pd_commons.h"

typedef struct ambiToBinaural
{
    // PD' stuff
    t_object x_obj;
    t_sample f;
    


    
    t_sample **m_ins;
    t_sample **m_outs;
    
    
    

    AmbisonicBinauralDecoder *m_decoder;

    
} t_ambiToBinaural;

static t_class *ambiToBinaural_class;


t_int *ambiToBinaural_perform(t_int *w)
{
    t_ambiToBinaural *x	= (t_ambiToBinaural *)(w[1]);
    

    x->m_decoder->process(x->m_ins, x->m_outs, x->m_decoder->getBufferSize());
    
    return (w + 2);
}



static void ambiToBinaural_dsp(t_ambiToBinaural *x, t_signal **sp, t_symbol *s)
{
    x->m_decoder->setConfig(sp[0]->s_n, sp[0]->s_sr);
    x->m_decoder->prepare();
    

    int i=0;
    for (; i<x->m_decoder->getNumOfHarmonics();i++)
        x->m_ins[i] = sp[i]->s_vec;
    
    

    // PD compte les e/s des objets dans le sens horaire:
    // m_outs[0] est la sortie droite de l'obj
    // m_outs[1] est la sortie gauche de l'obj
    // il faut donc inverser car le reste du code ( et de l'audio en
    // général comptent d'abord la gauche en 1er ...
    x->m_outs[0]= sp[i]->s_vec;
    x->m_outs[1]= sp[i+1]->s_vec;

    
    
    dsp_add(ambiToBinaural_perform, 1, x);
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
        if (argc>=2)
        {
            order   = atom_getint (argv);
            numVirtualSpeakers = atom_getint(argv+1);
            
            
        }
        
        if (order==0)
            order = 4;
        
        if (numVirtualSpeakers==0)
            numVirtualSpeakers = 8;
        
        post("config :");
        post("order : %i virtual speakers : %i",order,numVirtualSpeakers);
        
        
        x->m_decoder = new AmbisonicBinauralDecoder(order,numVirtualSpeakers);
        
        
        x->m_outs = new float*[2];
        
        x->m_ins = new float*[x->m_decoder->getNumOfHarmonics()];
        x->m_ins[0] = NULL;
        
        int i=1;
        
        for (/* i=1 */;i<x->m_decoder->getNumOfHarmonics();i++)
        {
            //signalinlet_new(&x->x_obj, x->f);
            inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
            x->m_ins[i]= NULL;
            
        }

        

        outlet_new(&x->x_obj, &s_signal);
        outlet_new(&x->x_obj, &s_signal);        
        x->m_outs[0] = NULL;
        x->m_outs[1] = NULL;

        
    }

    return x;

}

static void ambiToBinaural_free(t_ambiToBinaural *x)
{
    if (x)
    {

        delete[] x->m_outs;
        

        
        delete[] x->m_ins;
        
        delete x->m_decoder;
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
    
    CLASS_MAINSIGNALIN(ambiToBinaural_class, t_ambiToBinaural, f);
    
    
}


