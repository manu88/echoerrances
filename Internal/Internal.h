//
//  Internal.h
//  audioLib
//
//  Created by Manuel Deneu on 17/03/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __audioLib__Internal__
#define __audioLib__Internal__

#include <iostream>


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    RefCounted Object
 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class RefCountedObj
{
public:
    int retain() const
    {
        return ++m_refCount;
    }
    
    int release() const
    {
        return --m_refCount;
    }
    
    int getRefCount() const
    {
        return m_refCount;
    }
    
    
protected:
    RefCountedObj():
    m_refCount(1)
    {
        
    }
    
    
private:
    
    mutable int m_refCount;
};

#endif /* defined(__audioLib__Internal__) */