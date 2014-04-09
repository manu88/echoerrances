//
//  AudioTools.h
//  encoder~
//
//  Created by Manuel Deneu on 26/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef encoder__AudioTools_h
#define encoder__AudioTools_h

#include <math.h>

namespace AudioTools
{

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static void deleteMultiArrayFloat(float **array,int xSize)
    {
        int i = 0;
        for (;i<xSize;i++)
            delete [] array[i];
        
        delete [] array;
        
    }
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline float radianWrap(const float angle)
    {
        float _angle = angle;
        while(_angle < 0.)
            _angle += M_PI*2;
        while(_angle > M_PI*2)
            _angle -= M_PI*2;
        
        return _angle;
    }
        
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */    
    static void clipVal(float &val, float min,float max)
    {
        if (val>max)
            val=max;
        if (val<min)
            val=min;
    }





    
    
} // namespace AudioTools
#endif
