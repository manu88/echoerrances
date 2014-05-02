//
//  Audio_Basic.h
//  encoder~
//
//  Created by Manuel Deneu on 30/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

/*
 Ne pas inclure ce header directement! Utiliser FloatComputation.h
 */


#ifndef encoder__Audio_Basic_h
#define encoder__Audio_Basic_h

#include "../PDObjects/Debug_pd.h"

namespace FloatComputation
{


    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline void clearVector(float* vector,int size)
    {
//        memset(vector, 0, size * sizeof (float));

        int i;
        for (i=0 ; i<size; i++)
            vector[i] = 0.0;
    }

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline void copyVector(const float* src , float* dest, const int size)
    {

        int i;
        for (i=0 ; i<size; i++)
            dest[i] = src[i];

//        memcpy(dest, src, size * sizeof(float));

    }



    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline void multiplyByConstant(const float *input, float *output, float value, int size)
    {
        int i;
        for (i=0 ; i<size; i++)
        {
            output[i] = input[i] * value;
        }

    }

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline void multiply(const float *input1, const float *input2, float *output, int size)
    {
        int i;
        for (i=0 ; i<size; i++)
            output[i] = input1[i] * input2[i];
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
     //   pdAssert( (multiplier>=0.0) , "addWithMultiply : multiplier <0");
        int i=0;
        for (; i<num; i++)
        {
            dest[i] += src[i] * multiplier;
        }
    }

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static inline void add(const float* src, float* dest, int num)
    {
        int i=0;
        for (; i<num; i++)
        {
            dest[i] += src[i];
        }
    }

    static inline void convolve(float* input, size_t inputSize, float* impulse, size_t impulseSize, float* output)
    {
        int i = 0;
        for (; i<inputSize;i++) // bufferIN
            addWithMultiply(output+i+1, impulse, input[i], impulseSize);

    }

} // namespace


#endif
