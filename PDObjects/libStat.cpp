//
//  LibStats.c
//  audioLib
//
//  Created by Manuel Deneu on 15/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include <stdio.h>
#include "m_pd.h"
#include "obj_pd_commons.h"

static t_class *libStat_class;

typedef struct _counter
{
    t_object  x_obj;
    
    
} t_libStat;

/* **** **** **** **** **** **** **** **** */

static void libStat_dsp(t_libStat *x, t_signal **sp, t_symbol *s)
{

}

/* **** **** **** **** **** **** **** **** */

static void libStat_bang(t_libStat *x)
{
    
    libStats();
}



/* **** **** **** **** **** **** **** **** */

static void libStat_connect(t_libStat *x, t_floatarg c1,t_floatarg c2)
{
    const int from = c1;
    const int to   = c2;
    post("connect %f %f",c1,c2);
    
    if (GrandMaster::getMainAudioGraph()->connect(from, to))
        post("Ok");
    else
        post("failed");
}
/* **** **** **** **** **** **** **** **** */

static void libStat_nodes(t_libStat *x)
{
    int inputs  = 0;
    int outputs = 0;
    int effects = 0;
    post("nodes count %i",GrandMaster::getMainAudioGraph()->getNodeCount());

    int i = 0;
    for(; i<=AudioGraph::getCurrentId();i++)
    {
        AudioNode* node = GrandMaster::getMainAudioGraph()->getNodeForId(i);
        if (node)
        {
            post("*************************");
            post("got One node %i",node->getNodeId());
            
            if (node->getProcessor()->getType()==AudioProcessorInput)
            {
                post("its an input proc");
                inputs++;
            }
            else if (node->getProcessor()->getType()==AudioProcessorOutput)
            {
                post("its an output proc");
                outputs++;
            }
            else if (node->getProcessor()->getType()==AudioProcessorFX)
            {
                post("its an effect proc");
                effects++;
            }
            
            if (node->hasSource())
                post("inputs connected to node %i",node->getSource()->getNodeId());
            else
                post("inputs not connected");
                
            
            if (node->hasDestination())
                post("outputs connected to node %i",node->getDestinatation()->getNodeId());
            
            else
                post("outputs not connected ...");
                 
                 
        }
    }
    
    post("got %i input   procs",inputs);
    post("got %i effects procs",effects);
    post("got %i output  procs",outputs);
}

/* **** **** **** **** **** **** **** **** */

static void *libStat_new(void)
{
    t_libStat *x = (t_libStat *)pd_new(libStat_class);
    
    
    outlet_new(&x->x_obj, &s_float);
    
    return (void *)x;
}

/* **** **** **** **** **** **** **** **** */

extern "C" void libStat_setup(void)
{
    libStat_class = class_new(gensym("libStat"),
                              (t_newmethod)libStat_new,
                              0, sizeof(t_libStat),
                              CLASS_DEFAULT,
                              A_DEFFLOAT, 0);  
    
    class_addbang(libStat_class, libStat_bang);
    class_addmethod(libStat_class, (t_method)libStat_nodes, gensym("nodes"), A_NULL);
    class_addmethod(libStat_class, (t_method)libStat_dsp, gensym("dsp"), A_CANT);
    class_addmethod(libStat_class, (t_method)libStat_connect, gensym("connect"), A_FLOAT,A_FLOAT);
    
}