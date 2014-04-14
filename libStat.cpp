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

void libStat_bang(t_libStat *x)
{
    
    libStats();
}

void *libStat_new(void)
{
    t_libStat *x = (t_libStat *)pd_new(libStat_class);
    
    
    outlet_new(&x->x_obj, &s_float);
    
    return (void *)x;
}

extern "C" void libStat_setup(void)
{
    libStat_class = class_new(gensym("libStat"),
                              (t_newmethod)libStat_new,
                              0, sizeof(t_libStat),
                              CLASS_DEFAULT,
                              A_DEFFLOAT, 0);  
    
    class_addbang(libStat_class, libStat_bang);  
}