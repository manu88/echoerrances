
//
//  Binaural~.c
//  encoder~
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "m_pd.h"
#include "BinauralUtility.h"
#include "Binaural.h"
#include "FloatComputation.h"
#include "obj_pd_commons.h"

typedef struct binaural
{
    // PD' stuff
    t_object x_obj;
    t_sample f;
    
    t_sample **m_ins;
    t_sample **m_outs;
    
    

    AmbisonicBinauralDecoder *m_decoder;
    
    
    
    
    
} t_binaural;

static t_class *binaural_class;


t_int *binaural_perform(t_int *w)
{
    t_binaural *x	= (t_binaural *)(w[1]);
    
    x->m_decoder->process(x->m_ins, x->m_outs, x->m_decoder->getBufferSize());

    
    


    
    return (w + 2);
}



static void binaural_dsp(t_binaural *x, t_signal **sp, t_symbol *s)
{
    

    
    x->m_decoder->setConfig(sp[0]->s_n, sp[0]->s_sr);
    
    x->m_ins[0] = sp[0]->s_vec;
    

    for (int i = 0; i < 2; i++)
        x->m_outs[i]= sp[i+1]->s_vec;
    

    
    
    dsp_add(binaural_perform, 1, x);
}



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

static void *binaural_new(t_symbol *s, long argc, t_atom *argv)
{
    t_binaural *x = NULL;
    x = (t_binaural *)pd_new(binaural_class);
    if (x)
	{

        
        x->m_decoder = new AmbisonicBinauralDecoder(4,7,Small);
        
        
        x->m_outs = new float*[2];
        
        x->m_ins = new float*[1];
        x->m_ins[0] = NULL;

        for (int i = 0; i < 2; i++)
        {
			outlet_new(&x->x_obj, &s_signal);
            x->m_outs[i] = NULL;
        }
        
    }

    return x;

}

static void binaural_free(t_binaural *x)
{
    if (x)
    {
     
        delete x->m_decoder;
        delete[] x->m_outs;
        delete[] x->m_ins;        
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

extern "C" void binaural_tilde_setup(void)
{
    binaural_class = class_new(gensym("binaural~"),
                              (t_newmethod)binaural_new,
                              (t_method)binaural_free, sizeof(t_binaural),
                              CLASS_DEFAULT,
                              A_GIMME, 0);
    


    
    
    
    class_addmethod(binaural_class, (t_method)binaural_dsp, gensym("dsp"), A_CANT);
    
    CLASS_MAINSIGNALIN(binaural_class, t_binaural, f);
    
    
}
