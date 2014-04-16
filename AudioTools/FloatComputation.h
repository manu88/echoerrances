#ifndef simdAudioTest_FloatComputation_h
#define simdAudioTest_FloatComputation_h


#include "Config.h"

/*
 Definitions
 */
static inline void  clearVector(float* vector,int size);
static inline void  copyVector(const float* src , float* dest, const int size);
static inline void  multiplyByConstant(const float *input, float *output, float value, int size);
static inline void  multiply(const float *input1, const float *input2, float *output, int size);
static inline float vectors_dot_prod(const float *x, const float *y, int n);
static inline void  matrixByVector(const float **mat, const float *vec, float *result, int rows, int cols);

//! inverser source <-> dest dans les arguments
static inline void  addWithMultiply (float* dest, const float* src, float multiplier, int num);
static inline void  add(const float* src, float* dest, int num);
/*
 
 Todo : mettre ce qui n'est pas optimisable ici 
 (ex clearVector, etc.)
 
 */


/*
    Includes selon les modes de calcul supportés par l'architecture
 */

#ifdef USE_BASIC_COMPUTATION
    #include "Audio_Basic.h"

#elif defined __arm__
    #include "Audio_Neon.h"

#elif defined  __APPLE__
    #include "Audio_Darwin.h"

#endif


#endif
