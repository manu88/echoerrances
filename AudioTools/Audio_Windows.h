//
//  Audio_Windows.h
//  encoder~
//
//  Created by Paul Payen de La Garanderie on 22/04/14 (based on Audio_Darwin.h created by Manuel Deneu, version of 22/04/14).
//  Copyright (c) 2014 Manuel Deneu and Paul Payen de La Garanderie. All rights reserved.
//

/*
 Ne pas include ce header directement! Utiliser FloatComputation.h
 */

#ifndef encoder__Audio_Windows_h
#define encoder__Audio_Windows_h


#include <xmmintrin.h>
#include <cstring>
#include <cstdlib>
#include "../Internal/align.h"

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
        memcpy(dest, src, sizeof(float)*size);
/*
        __m128 t0;

        for (int i=0;i<size; i+=4)
        {

            t0 = _mm_load_ps(src+i);
            _mm_store_ps(dest+i, t0);

        }*/
/*
        int i;
        for (i=0 ; i<size; i++)
            dest[i] = src[i];
*/
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
/*
        __m128 t0, t1;

        t1 = _mm_load1_ps(&value);

        for (int i=size; i-=4;)
        {

            t0 = _mm_load_ps(input+i);


            t0 = _mm_mul_ps(t0, t1);

            _mm_store_ps(output+i, t0);

        }
*/

    }

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

    static inline void multiply(const float *input1, const float *input2, float *output, int size)
    {

        int i;
        for (i=0 ; i<size; i++)
            output[i] = input1[i] * input2[i];
        /*
        __m128 t0, t1;

        for (int i=size; i-=4;)
        {

            t0 = _mm_load_ps(input1+i);
            t1 = _mm_load_ps(input2+i);
            t0 = _mm_mul_ps(t0, t1);

            _mm_store_ps(output+i, t0);

        }
        */
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
        /*
        // version pas optimisée
        double res = 0.0;
        int i = 0;
        for (; i <= n-4; i+=4)
        {

            float _r[4] = {0.,0.,0.,0.};
            __m128 t0, t1;

            t0 = _mm_load_ps(x+i);
            t1 = _mm_load_ps(y+i);
            t0 = _mm_mul_ps(t0, t1);

            _mm_store_ps(_r, t0);

            res +=_r[0] + _r[1] +_r[2] +_r[3];

//             res += (x[i] * y[i] +
//             x[i+1] * y[i+1] +
//             x[i+2] * y[i+2] +
//             x[i+3] * y[i+3]);

        }

        for (; i < n; i++)
        {
            res += x[i] * y[i];
        }
        return res;
        */
    }

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

    static inline void  matrixByVector(const float **mat, const float *vec, float *result, int rows, int cols)
    { // in matrix form: result = mat * vec;
        int i;
        for (i = 0; i < rows; i++)
        {
            float* _mat = (float*) aligned_alloc(16, sizeof(float)*cols);
            memcpy(_mat, mat[i], sizeof(float)*cols);
            result[i] = vectors_dot_prod(_mat, vec, cols);
            aligned_free(_mat);
        }
    }
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

    static inline void addWithMultiply (float* dest, const float* src, float multiplier, int num)
    {
        int i=num;
        for (; i--;)
        {
            dest[i] += src[i] * multiplier;
        }
/*

        __m128 t0, t1, t2 , t3 , t4;

        t1 = _mm_set1_ps(multiplier);


        for (; i <= num-4; i+=4)
        {

            t0 = _mm_load_ps(src+i);
            t3 =  _mm_load_ps(dest+i);

            t2 =  _mm_mul_ps(t0, t1);
            t4 = _mm_add_ps(t3, t2);
            _mm_store_ps(dest+i, t4);



        }
 */
    }

    static inline void  add(const float* src, float* dest, int num)
    {


        int i=num;
        for (; i--;)
        {
            dest[i] += src[i];
        }
    }

    static inline void convolve(float* input, size_t inputSize, float* impulse, size_t impulseSize, float* output)
        //All pointer should be aligned in memory by 16
    {
        input-=4;                                   //on déplace de 4 flottants vers la gauche le bloc (pour optimisation)

        // chargement des paquets initiaux de quatre nombres flottants du block à convoluer" (variable : bloc)
        __m128 abcd = _mm_set1_ps(0);               //[a,b,c,d]=[0,0,0,0]
        __m128 efgh = _mm_load_ps(input+4);         //[e,f,g,h] (le chargement s'effectue en dehors de la boucle
                                                    //           car au dernier tour il faut que ce registre soit nul)

        unsigned int i = 0;
        while(i<=inputSize)     //bouclage sur la liste de donnée
        {
            // empaquetage des données pour faciliter la multplication et surtout l'addition
            __m128 matl1 = _mm_shuffle_ps(abcd, efgh, _MM_SHUFFLE(2,0,2,0));    //matl1=[a,c,e,g]

            __m128 matl2 = _mm_shuffle_ps(matl1, matl1, _MM_SHUFFLE(2,1,2,1));  //matl2=[c,e,c,e]

            __m128 matl3 = _mm_shuffle_ps(efgh, abcd, _MM_SHUFFLE(3,1,1,0));    //matl3=[e,f,b,d]
            matl3 = _mm_shuffle_ps(matl3, matl3, _MM_SHUFFLE(3,1,3,2));         //matl3=[b,d,f,d]

            __m128 matl4 = _mm_shuffle_ps(matl3, matl3, _MM_SHUFFLE(2,1,0,1));  //matl4=[d,b,d,f]

            for(unsigned int j=0; j<impulseSize; j+=4)    //bouclage sur la liste de la réponse impulsionnelle
            {
                //chargement de la réponse impulsionnelle
                __m128 vect = _mm_load_ps(impulse+j);

                //multiplicationns
                __m128 matl1m = _mm_mul_ps(matl1, vect);    //matl1m = [ax,cy,ez,gw]
                __m128 matl2m = _mm_mul_ps(matl2, vect);    //matl2m = [cx,ey,cz,ew]
                __m128 matl3m = _mm_mul_ps(matl3, vect);    //matl3m = [bx,dy,fz,dw]
                __m128 matl4m = _mm_mul_ps(matl4, vect);    //matl4m = [dx,by,dz,fw]

                //empaquetage des données pour l'addition
                matl2m = _mm_shuffle_ps(matl2m, matl2m, _MM_SHUFFLE(1,0,3,2));  //matl2m = [cz,ew,cx,ey]
                matl3m = _mm_shuffle_ps(matl3m, matl3m, _MM_SHUFFLE(2,1,0,3));  //matl3m = [dw,bx,dy,fz]
                matl4m = _mm_shuffle_ps(matl4m, matl4m, _MM_SHUFFLE(0,3,2,1));  //matl4m = [by,dz,fw,dx]

                //additions
                __m128 sum = _mm_add_ps(matl1m, matl2m);    //sum = [ax+ew,cy+cx,ez+ey,gw+cz]
                sum = _mm_add_ps(matl3m, sum);              //sum = [ax+ew+fz,cy+cx+dw,ez+ey+bx,gw+cz+dy]
                sum = _mm_add_ps(matl4m, sum);              //sum = [ax+ew+fz+by,
                                                            //       cy+cx+dw+dz,
                                                            //       ez+ey+bx+fw,
                                                            //       gw+cz+dy+dx]

                //chargement et ajout à ce qui a déjà été calculé (variable : out1)
                __m128 res = _mm_load_ps(output+j+i);   //res = [m,n,o,p]
                sum = _mm_add_ps(res, sum);             //sum = [ax+ew+fz+by+m,
                                                        //       cy+cx+dw+dz+n,
                                                        //       ez+ey+bx+fw+o,
                                                        //       gw+cz+dy+dx+p]

                //enregistrement du résultat final
                _mm_store_ps(output+j+i, sum);
            }

            //calcul des variables pour le prochain tour
            if(i>=inputSize)    //on stop la boucle si on a tout calculé
                break;
            i+=4;               //sinon on se déplace de 4 nombres pour le calcul de la convolution sur les données
            abcd = _mm_load_ps(input+i);    //on charge les quatres premiers nombres (sur 8) pour le calcul
            if(i<inputSize)                 //s'il reste encore des nombres après on charge les quatre suivants
                efgh = _mm_load_ps(input+i+4);
            else                            //sinon c'est que c'est le dernier tour de boucle, et qu'il faut donc charger des 0
                efgh = _mm_set1_ps(0);
        }
    }


} // namespace

#endif

