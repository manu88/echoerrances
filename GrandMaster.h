//
//  GrandMaster.h
//  audioLib
//
//  Created by Manuel Deneu on 14/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __audioLib__GrandMaster__
#define __audioLib__GrandMaster__

#include <iostream>
#include "Internal.h"
#include "AudioInternals.h"

/*
 
 Singleton (Use retain/release)
 
 */
class GrandMaster : RefCountedObj
{
public:
    static GrandMaster* getInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = new GrandMaster();
            s_instance->RefCountedObj::release();
        }
        
        return s_instance;
    }
    
    static void retain()
    {
        getInstance()->RefCountedObj::retain();
    }
    
    static void release()
    {
        s_instance->RefCountedObj::release();
        
        if (s_instance->getRefCount() == 0)
        {
            delete s_instance;
            s_instance = nullptr;
        }
    }
    
    static int getRefCount()
    {
        return getInstance()->RefCountedObj::getRefCount();
    }
    
    static AudioGraph* getMainAudioGraph()
    {
        return getInstance()->m_graph;
    }


    
    
private:
    GrandMaster();
    ~GrandMaster();
    
    AudioGraph* m_graph;
    
    
    static GrandMaster* s_instance;
    
};

#endif /* defined(__audioLib__GrandMaster__) */
