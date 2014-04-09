//
//  AmbisonicUtility.h
//  encoder~
//
//  Created by Manuel Deneu on 25/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __encoder___AmbisonicUtility__
#define __encoder___AmbisonicUtility__

#include <list>
#include "AudioTools.h"
#include "Internal.h"

#define CICM_1OVER2PI_RATIO 5729.41879637

enum {numberOfCirclePoints = 36000};



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
 Ambisonic (2D): Base class
 */
class InternalAmbisonic : public RefCountedObj
{
public:
    
    int getOrder() const
    {
        return m_order;
    }
    
    int getHarmonicNumber() const
    {
        return m_numberOfHarmonics;
    }
    
protected :
    InternalAmbisonic(int order) :
        m_order(order),
        m_numberOfHarmonics(order*2 +1)
    {
    }
    
    ~InternalAmbisonic()
    {
        
    }

    int  getHarmonicIndex(int index);
    int  getHarmonicOrder(int index);
    
    int m_order;
    int m_numberOfHarmonics;    
};

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*

 AmbisonicEncoder
 
 Ne pas utiliser directement. Utiliser la classe AmbisonicEncoder (Ambisonic.h)
 
 */
class InternalAmbisonicEncoder : public InternalAmbisonic
{
public :
    InternalAmbisonicEncoder(int order);
    ~InternalAmbisonicEncoder();
    
    float** getEncodingMatrix() const
    {
        return m_encoderMatrix ;
    }
    
    void computeEncodingMatrix();



private:
    float** m_encoderMatrix;
    
};

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
 AmbisonicDecoder
 
 Ne pas utiliser directement. Utiliser la classe AmbisonicDecoder (Ambisonic.h)
 
 */
class InternalAmbisonicDecoder : public InternalAmbisonic
{
public:
    InternalAmbisonicDecoder(int order, int numberOfOutputs);
    ~InternalAmbisonicDecoder();
    
    float** getDecodingMatrix() const
    {
        return m_decoderMatrix ;
    }
    
    void computeDecodingMatrix();
    
    int getNumberOfOutputs() const
    {
        return m_outputCount;
    }

private:
    int     m_outputCount;
    float** m_decoderMatrix;
    

};


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    SINGLETON
 
    Gestion & partage des ressources ambisoniques. 
    Si une matrice d'encodage est déjà existante pour un ordre donnée, elle sera 
    partagé afin d'économiser les ressources.
 
 
    PBs : 
        - Pure data ne partage pas de mémoire entre différents externals -> il existe donc
          2 instances de AmbisonicUtility, une pour les encodeurs, une pour les décodeurs....
 
 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class AmbisonicUtility
{
public:
    /* STATIC*/    
    
    static AmbisonicUtility* getInstance()
    {
        if (s_instance==nullptr)
            s_instance = new AmbisonicUtility();
        
        return s_instance;
    }

    static int getRefCount()
    {
        return s_refCount;
    }
    
    static int retainInstance()
    {
        return s_refCount++;
        

    }
    
    static int releaseInstance()
    {
        s_refCount--;
        
        if (s_refCount==0)
        {
            delete s_instance;
            s_instance = nullptr;
        }
        
        return s_refCount;
    }
    
    static InternalAmbisonicEncoder* getEncoder(int order)
    {
        return getInstance()->getEncoderForOrder(order);
    }
    
    
    static InternalAmbisonicDecoder* getDecoder(int order,int numberOfOutputs)
    {
        return getInstance()->getDecoderForOrder(order, numberOfOutputs);
    }


    
    
    // supprime les encodeurs dont le refCount=0
    void inspectEncoderList();

    // supprime les decodeurs dont le refCount=0
    void inspectDecoderList();

    /*
     Debug, stats
     */
    static int getEncoderCountForOrder(int order)
    {
//        return s_instance->m_encoderList.size();
        if (s_instance->containsEncoder(order) )
            return s_instance->getEncoderForOrder(order,false/* NO RETAIN, just checkin'*/)->getRefCount();
        
        return -1;
    }
    
    static int getDecoderCount()
    {
        return s_instance->m_decoderList.size();
    }
    
    static int getEncoderCount()
    {
        return s_instance->m_encoderList.size();
    }
    


protected:
    
    AmbisonicUtility();
    ~AmbisonicUtility();
    
    // renvoie l'encodeur s'il existe, et le créée si besoin
    InternalAmbisonicEncoder* getEncoderForOrder(int order, bool retain = true);
    
    // renvoie le decodeur s'il existe, et le créée si besoin
    InternalAmbisonicDecoder* getDecoderForOrder(int order , int numberOfOutputs,bool retain = true);
    
    
    // revoie l'encodeur s'il le trouve dans m_encoderList, nullptr sinon
    InternalAmbisonicEncoder* containsEncoder(int order);
    
    // revoie le décodeur s'il le trouve dans m_decoderList, nullptr sinon
    InternalAmbisonicDecoder* containsDecoder(int order , int numOuts);
    
    
private:
    /* INSTANCE */
    // liste des matrices d'encodage dispo
    std::list<InternalAmbisonicEncoder*> m_encoderList;

    // liste des matrices de décodage dispo
    std::list<InternalAmbisonicDecoder*> m_decoderList;
    

    
    /* STATIC*/
    static int                s_refCount;
    static AmbisonicUtility * s_instance;
};

#endif /* defined(__encoder___AmbisonicUtility__) */
