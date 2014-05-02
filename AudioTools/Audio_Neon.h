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


#include <arm_neon.h>
#include <string>

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
        // version pas optimisŽe
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

/*
    static inline void convolve(float* input, size_t inputSize, float* impulse, size_t impulseSize, float* output)
        //All pointer should be aligned in memory by 16
    {
        input-=4;                                   //on déplace de 4 flottants vers la gauche le bloc (pour optimisation)

        // chargement des paquets initiaux de quatre nombres flottants du block à convoluer" (variable : bloc)
        float32x4_t abcd = vdupq_n_f32(0);               //[a,b,c,d]=[0,0,0,0]
        float32x4_t efgh = vld1q_f32(input+4);         //[e,f,g,h] (le chargement s'effectue en dehors de la boucle
                                                    //           car au dernier tour il faut que ce registre soit nul)

        unsigned int i = 0;
        while(i<=inputSize)     //bouclage sur la liste de donnée
        {
            // empaquetage des données pour faciliter la multplication et surtout l'addition
            //float32x4_t matl1 = _mm_shuffle_ps(abcd, efgh, _MM_SHUFFLE(2,0,2,0));    //matl1=[a,c,e,g]
            float32x4x2_t element1 = vuzpq_f32(abcd, efgh);

            //float32x4_t matl2 = _mm_shuffle_ps(matl1, matl1, _MM_SHUFFLE(2,1,2,1));  //matl2=[c,e,c,e]
            float32x2x2_t element2 = vuzpq_f32(vget_high_f32(abcd), vget_low_f32(efgh));

            //float32x4_t matl3 = _mm_shuffle_ps(efgh, abcd, _MM_SHUFFLE(3,1,1,0));    //matl3=[e,f,b,d]

            //matl3 = _mm_shuffle_ps(matl3, matl3, _MM_SHUFFLE(3,1,3,2));         //matl3=[b,d,f,d]
            float32x4 element3 = vrev64q_f32(vcombine_f32(vget_low_f32(element1.val[1]), element2.val[1]));

            //float32x4_t matl4 = _mm_shuffle_ps(matl3, matl3, _MM_SHUFFLE(2,1,0,1));  //matl4=[d,b,d,f]

            float32x4 matl1 = element1.val[0];
            float32x4 matl2 = vcombine_f32(element2[0], element2[0]);
            float32x4 matl3 = vcombine_f32(vget_low_f32(element1.val[0]), vget_high_f32(element3));
            float32x4 matl4 = vcombine_f32(vget_low_f32(element3), element2[1]);

            for(unsigned int j=0; j<impulseSize; j+=4)    //bouclage sur la liste de la réponse impulsionnelle
            {
                //chargement de la réponse impulsionnelle
                float32x4_t vect = vld1q_f32(impulse+j);

                //multiplicationns
                float32x4_t matl1m = vmulq_f32(matl1, vect);    //matl1m = [ax,cy,ez,gw]
                float32x4_t matl2m = vmulq_f32(matl2, vect);    //matl2m = [cx,ey,cz,ew]
                float32x4_t matl3m = vmulq_f32(matl3, vect);    //matl3m = [bx,dy,fz,dw]
                float32x4_t matl4m = vmulq_f32(matl4, vect);    //matl4m = [dx,by,dz,fw]

                //empaquetage des données pour l'addition
                matl2m = _mm_shuffle_ps(matl2m, matl2m, _MM_SHUFFLE(1,0,3,2));  //matl2m = [cz,ew,cx,ey]
                matl3m = _mm_shuffle_ps(matl3m, matl3m, _MM_SHUFFLE(2,1,0,3));  //matl3m = [dw,bx,dy,fz]
                matl4m = _mm_shuffle_ps(matl4m, matl4m, _MM_SHUFFLE(0,3,2,1));  //matl4m = [by,dz,fw,dx]

                //# additions
                float32x4_t sum = vaddq_f32(matl1m, matl2m);    //sum = [ax+ew,cy+cx,ez+ey,gw+cz]
                sum = vaddq_f32(matl3m, sum);              //sum = [ax+ew+fz,cy+cx+dw,ez+ey+bx,gw+cz+dy]
                sum = vaddq_f32(matl4m, sum);              //sum = [ax+ew+fz+by,
                                                            //       cy+cx+dw+dz,
                                                            //       ez+ey+bx+fw,
                                                            //       gw+cz+dy+dx]

                //# chargement et ajout à ce qui a déjà été calculé (variable : out1)
                float32x4_t res = vld1q_f32(output+j+i);   //res = [m,n,o,p]
                sum = vaddq_f32(res, sum);             //sum = [ax+ew+fz+by+m,
                                                        //       cy+cx+dw+dz+n,
                                                        //       ez+ey+bx+fw+o,
                                                        //       gw+cz+dy+dx+p]

                //enregistrement du résultat final
                vst1q_f32(output+j+i, sum);
            }

            //# calcul des variables pour le prochain tour
            if(i>=inputSize)    //on stop la boucle si on a tout calculé
                break;
            i+=4;               //sinon on se déplace de 4 nombres pour le calcul de la convolution sur les données
            abcd = vld1q_f32(input+i);    //on charge les quatres premiers nombres (sur 8) pour le calcul
            if(i<inputSize)                 //s'il reste encore des nombres après on charge les quatre suivants
                efgh = vld1q_f32(input+i+4);
            else                            //sinon c'est que c'est le dernier tour de boucle, et qu'il faut donc charger des 0
                efgh = vld1q_f32(0);
        }
    }*/


    static inline void convolve(float* input, size_t inputSize, float* impulse, size_t impulseSize, float* output)
        //All pointer should be aligned in memory by 16
    {
        input-=4;                                   //on déplace de 4 flottants vers la gauche le bloc (pour optimisation)

        // chargement des paquets initiaux de quatre nombres flottants du block à convoluer" (variable : bloc)
        float32x4_t abcd = vdupq_n_f32(0);               //[a,b,c,d]=[0,0,0,0]
        float32x4_t efgh = vld1q_f32(input+4);         //[e,f,g,h] (le chargement s'effectue en dehors de la boucle
                                                    //           car au dernier tour il faut que ce registre soit nul)

        unsigned int i = 0;
        while(i<=inputSize)     //bouclage sur la liste de donnée
        {
            // empaquetage des données pour faciliter la multplication et surtout l'addition

            float32x4_t matl1 = efgh;
            float32x4_t matl3 = vcombine_f32(vget_high_f32(abcd), vget_low_f32(efgh));

            uint8x16_t abcd8 = vreinterpretq_u8_f32(abcd);
            uint8x16_t efgh8 = vreinterpretq_u8_f32(efgh);
            uint8x8x4_t elementx4;
            elementx4.val[0] = vget_low_u8(abcd8);
            elementx4.val[1] = vget_high_u8(abcd8);
            elementx4.val[2] = vget_low_u8(efgh8);
            elementx4.val[3] = vget_high_u8(efgh8);

            uint8x8_t element1 = vtbl4_u8(elementx4, vcreate_u8(0x0405060708090A0B));
            uint8x8_t element2 = vtbl4_u8(elementx4, vcreate_u8(0x0C0D0E0F10111213));
            uint8x8_t element3 = vtbl4_u8(elementx4, vcreate_u8(0x1415161718191A1B));

            float32x4_t matl2 = vreinterpretq_f32_u8(vcombine_u8(element1, element2));
            float32x4_t matl4 = vreinterpretq_f32_u8(vcombine_u8(element2, element3));

            for(unsigned int j=0; j<impulseSize; j+=4)    //bouclage sur la liste de la réponse impulsionnelle
            {
                //chargement de la réponse impulsionnelle
                float32x2_t vect1 = vld1_f32(impulse+j);
                float32x2_t vect2 = vld1_f32(impulse+j+2);

                float32x4_t sum = vld1q_f32(output+j+i);   //res = [m,n,o,p]

                sum = vmlaq_lane_f32(matl1, sum, vect1, 0);
                sum = vmlaq_lane_f32(matl2, sum, vect1, 1);
                sum = vmlaq_lane_f32(matl3, sum, vect2, 0);
                sum = vmlaq_lane_f32(matl4, sum, vect2, 1);

                //enregistrement du résultat final
                vst1q_f32(output+j+i, sum);
            }

            //# calcul des variables pour le prochain tour
            if(i>=inputSize)    //on stop la boucle si on a tout calculé
                break;
            i+=4;               //sinon on se déplace de 4 nombres pour le calcul de la convolution sur les données
            abcd = vld1q_f32(input+i);    //on charge les quatres premiers nombres (sur 8) pour le calcul
            if(i<inputSize)                 //s'il reste encore des nombres après on charge les quatre suivants
                efgh = vld1q_f32(input+i+4);
            else                            //sinon c'est que c'est le dernier tour de boucle, et qu'il faut donc charger des 0
                efgh = vld1q_f32(0);
        }
    }

} // namespace

#endif
