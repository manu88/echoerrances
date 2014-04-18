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
#include "Config.h"
#include <assert.h>




static void pdAssert(bool cond, const char *err, int num = 123456)
{
#ifdef PD_DEBUG
    if (!cond)
    {
        if (num==123456)
            error("Assertion ERROR : %s",err);
        else
            error("Assertion ERROR : %s %i",err,num);
    }
    
    
    //assert(cond);
#endif
}



#endif
