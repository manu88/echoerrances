//
//  AudioTools.h
//  encoder~
//
//  Created by Manuel Deneu on 26/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef encoder__AudioTools_h
#define encoder__AudioTools_h

//#ifdef WIN32
//#define _USE_MATH_DEFINES
//#endif // WIN32

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

#include "../PDObjects/Debug_pd.h"

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
        /*
        float _angle = angle;
        while(_angle < 0.)
            _angle += M_PI*2;
        while(_angle > M_PI*2)
            _angle -= M_PI*2;

        return _angle;
         */
        return fmod(angle, 2*M_PI);
    }

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    static void clipVal(float &val, float min,float max)
    {
        if (val>max)
            val=max;
        if (val<min)
            val=min;
    }

    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
    class LinearInterPolator
    {
    public:
        LinearInterPolator(float from, float to)
        {
            m_start = from;
            m_end = to;
            m_currentPos = from;
            m_dir = 1;
        }
        ~LinearInterPolator()
        {

        }

        void changeDestination(float newEndPoint)
        {
            m_start = m_currentPos;
            m_end   = newEndPoint;

            if (m_end<m_start)
            {
                m_dir =-1;
                float temp   = m_start;
                m_start      = m_end;
                m_end        = temp;
                m_currentPos = m_start;
            }
            else
                m_dir = 1;


        }

        float incPosition()
        {
            m_currentPos+=(0.002)*m_dir;
            clipVal(m_currentPos, m_start, m_end);

            return m_currentPos;
        }

        float getPosition() const
        {
            return m_currentPos;
        }

        float getDestination() const
        {
            return m_end;
        }

        float getStartPoint() const
        {
            return m_start;
        }

    private:
        float m_currentPos;
        float m_start;
        float m_end;

        int   m_dir;
    };
    /* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */




} // namespace AudioTools
#endif
