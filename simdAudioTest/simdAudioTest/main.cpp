//
//  main.c
//  simdAudioTest
//
//  Created by Manuel Deneu on 27/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include <stdio.h>

#include "../../FloatComputation.h"
#include <time.h>


void testCopyVector()
{
    const int size = 2048;
    float *input = new float[size];
    float *output = new float[size];
    

    for (int i=size;i--;)
    {
        input[i]  = 1.0;
        output[i] = 0.0;
    }
    
    
    int loop;    
    for (loop = 0; loop<80;loop++)
    {

        

        
        unsigned long startTime = clock();
        
        FloatComputation::multiplyByConstant(input, output, 32., size);

        
        unsigned long endTime = clock();


        
        unsigned long tickTime = endTime - startTime;
        float time = (float)tickTime/CLOCKS_PER_SEC;
//        printf("\n time = %f",time);
        printf("\n ticks = %lu",tickTime);
    }
    delete[] input;
    delete[] output;

}

void testMatrix()
{
    const int rows = 18;
    const int columns = 16;
    
    float **mat = new float*[rows];
    for (int i =0; i<rows;i++)
        mat[i] = new float[columns];
    
    
    for (int i =0; i<rows;i++)
        for (int j =0; j<columns;j++)
        {
            if (i==j)
                mat[i][j] = 1.0;
            else
                mat[i][j] = 0.;
        }
    
    float* in = new float[columns];
    float* out = new float[columns];
    for (int j =0; j<columns;j++)
    {
        in[j] = 5;
        out[j] = 1.;
    }
    unsigned long startTime = clock();
    
    
    FloatComputation::matrixByVector(const_cast<const float**>(mat), in, out, rows, columns);
    
    unsigned long endTime = clock();
    
    printf("\n MAT \n");
    for (int i =0; i<rows;i++)
    {
        for (int j =0; j<columns;j++)
            printf(" %f ",mat[i][j]);
        printf("\n");
    }
    printf("\n OUT \n");
    for (int j =0; j<columns;j++)
        printf(" %f ",out[j]);
    
    printf("\n IN \n");
    
    for (int j =0; j<columns;j++)
        printf(" %f ",in[j]);
    
    for (int i =0; i<rows;i++)
        delete [] mat[i];
    
    
    delete[] mat;
    delete [] in;
    delete [] out;
    
    
    unsigned long tickTime = endTime - startTime;
    float time = (float)tickTime/CLOCKS_PER_SEC;
    printf("\n time = %f",time);
    printf("\n ticks = %lu",tickTime);
    // 0.000002 secondes sans SIMD

}

int main(int argc, const char * argv[])
{
    testCopyVector();
    return 0;
}

