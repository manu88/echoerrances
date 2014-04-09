//
//  Audio_Darwin.h
//  encoder~
//
//  Created by Manuel Deneu on 25/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

/*
 Ne pas include ce header directement! Utiliser FloatComputation.h
 */

#ifndef encoder__Audio_Darwin_h
#define encoder__Audio_Darwin_h


#include <immintrin.h>
#include <string>

namespace FloatComputation
{
        
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline void clearVector(float* vector,int size)
    {
        memset(vector, 0, size * sizeof (float));
    }
            
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline void copyVector(const float* src , float* dest, const int size)
    {
/*
        __m128 t0;
        
        for (int i=0;i<size; i+=4)
        {
            
            t0 = _mm_load_ps(src+i);
            _mm_store_ps(dest+i, t0);
            
        }
*/
        int i;
        for (i=0 ; i<size; i++)
            dest[i] = src[i];

    }
    
        
        
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

    static inline void multiplyByConstant(const float *input, float *output, float value, int size)
    {

        __m128 t0, t1; 

        t1 = _mm_load1_ps(&value);
        
        for (int i=size; i-=4;)
        {
            
            t0 = _mm_load_ps(input+i);

            
            t0 = _mm_mul_ps(t0, t1);
            
            _mm_store_ps(output+i, t0);
            
        }

      
    }
        
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
        
    static inline void multiply(const float *input1, const float *input2, float *output, int size)
    {
        __m128 t0, t1;
        
        for (int i=size; i-=4;)
        {
            
            t0 = _mm_load_ps(input1+i);
            t1 = _mm_load_ps(input2+i);
            t0 = _mm_mul_ps(t0, t1);
            
            _mm_store_ps(output+i, t0);
            
        }
    }
        
        
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
        
    static inline float vectors_dot_prod(const float *x, const float *y, int n)
    {
        // version pas optimisée
        double res = 0.0;
        int i = 0;
        for (; i <= n-4; i+=4)
        {
    /*
            float _r[4] = {0.,0.,0.,0.};
            __m128 t0, t1;
            
            t0 = _mm_load_ps(x+i);
            t1 = _mm_load_ps(y+i);
            t0 = _mm_mul_ps(t0, t1);
            
            _mm_store_ps(_r, t0);

            res +=_r[0] + _r[1] +_r[2] +_r[3];
    */
             res += (x[i] * y[i] +
             x[i+1] * y[i+1] +
             x[i+2] * y[i+2] +
             x[i+3] * y[i+3]);

        }
        for (; i < n; i++)
        {
            res += x[i] * y[i];
        }
        return res;
    }
        
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
        
    static inline void  matrixByVector(const float **mat, const float *vec, float *result, int rows, int cols)
    { // in matrix form: result = mat * vec;
        int i;
        for (i = 0; i < rows; i++)
        {
            result[i] = vectors_dot_prod(mat[i], vec, cols);
        }
    }
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
        
    static inline void addWithMultiply (float* dest, const float* src, float multiplier, int num)
    {
        int i=0;
        for (; i<num; i++)
        {
            dest[i] += src[i] * multiplier;
        }
    }
    
    
} // namespace

#endif
