//
//  Audio_Neon.h
//  simdAudioTest
//
//  Created by Manuel Deneu on 30/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

/*
 Ne pas include ce header directement! Utiliser FloatComputation.h
 */

#ifndef simdAudioTest_Audio_Neon_h
#define simdAudioTest_Audio_Neon_h

namespace FloatComputation
{


    static inline void multiplyByConstant(const float *input, float *output, float value, int size)
    {
        int i;
        
        float32x4_t a ,b;
        
        for (i = size;i-=4;)
        {
            a = vld1q_f32 (input + i);
            
            b = vmulq_n_f32( a , value);
            
            vst1q_f32(output+i , b);
        }
    }



} // namespace

#endif
