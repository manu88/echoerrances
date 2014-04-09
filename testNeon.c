#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>
#include <time.h>

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


int main()
{
    const int size = 2048;;

    float *input = (float*) malloc(size * sizeof(float) );
    float *output = (float*) malloc(size * sizeof(float) );

    int i , loop;
    for (i=size;i--;)
    {
        input[i]  = 1.0;
        output[i] = 0.0;
    }

for (loop = 0; loop<80; loop++)
{

unsigned long startTime = clock();

    multiplyByConstant(input, output, 32., size);

unsigned long endTime = clock();

    unsigned long tickTime = endTime - startTime;
    printf("\n ticks = %lu \n",tickTime);
}
    free(input);
    free(output);

    return 0;
}
