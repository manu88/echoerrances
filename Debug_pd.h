//
//  Debug_pd.h
//  audioLib
//
//  Created by Manuel Deneu on 03/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef audioLib_Debug_pd_h
#define audioLib_Debug_pd_h

#include "m_pd.h"


#ifdef DEBUG 
#undef DEBUG
#endif

// debug mode
#define DEBUG

static void pdAssert(bool cond, const char *err)
{
#ifdef DEBUG
    if (!cond)
        error("Assertion ERROR : %s",err);
#endif
}



#endif
