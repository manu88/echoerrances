//
//  AudioInternals.h
//  audioLib
//
//  Created by Manuel Deneu on 17/03/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#ifndef __audioLib__AudioInternals__
#define __audioLib__AudioInternals__

#include <list>

class AudioProcessorBase; //forward

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
 
    Tentative de remplacement du DSP tree de PD par un autre interne à la bibliothèque ...
    En gros, il n'y a plus qu'un fil (data, pas audio) entre chaque objet qui remplace les (trop nombreuses) connections audio.
 */

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class AudioNode
{
public:
    AudioNode(AudioProcessorBase *proc , int nodeId);
    ~AudioNode();
    
    int getNodeId() const
    {
        return m_nodeId;
    }
    
    AudioProcessorBase *getProcessor() const
    {
        return m_ownedProcessor;
    }
    
    AudioNode* getSource() const
    {
        return m_from;
    }
    
    void setSource(AudioNode *node)
    {
        m_from = node;
    }

    AudioNode* getDestinatation() const
    {
        return m_to;
    }
    
    void setDestinatation(AudioNode *node)
    {
        m_to = node;
    }
    
    bool hasSource() const
    {
        return m_from != nullptr;
    }
    bool hasDestination() const
    {
        return m_to != nullptr;
    }
    
    
protected:
    int                 m_nodeId;
    
    AudioProcessorBase *m_ownedProcessor;

    AudioNode          *m_from;
    AudioNode          *m_to;
    
};

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
class AudioGraph
{
public:
    AudioGraph();
    ~AudioGraph();
    
    int addNode(AudioProcessorBase* procToAdd);
    bool removeNode(int nodeIdToRemove);
    
    bool connect(int sourceNode , int destNode);
    
    int getNodeCount() const
    {
        return m_nodesList.size();
    }
    
    
    AudioNode* getNodeForId(int nodeId);
    
    static void resetIdCounter()
    {
        s_idCounter = 0;
    }
    
    static int getNextId()
    {
        return ++s_idCounter;
    }
    
    static int getCurrentId() 
    {
        return s_idCounter;
    }
private:
    
    std::list<AudioNode*> m_nodesList;
    

    
    
    static int s_idCounter;
};


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
 Base class for audio processors
 */

typedef enum
{
    AudioProcessorInput, // Buffers have to be created for max(numIn;numOut)
    AudioProcessorOutput,
    AudioProcessorFX     //
}AudioProcessorType;


class AudioProcessorBase
{
public:

    int getBufferSize() const
    {
        return m_bufferSize;
    }
    
    void setBufferSize(int newBufferSize)
    {
        m_bufferSize = newBufferSize;
    }
    
    double getSampleRate() const
    {
        return m_sampleRate;
    }
    
    void setSampleRate(double newSampleRate)
    {
        m_sampleRate = newSampleRate;
    }
    
    void setConfig(int bufferSize, double sampleRate);
    
    AudioProcessorType getType() const
    {
        return m_type;
    }
    
    virtual inline void process(float **ins, float **outs, int bufferSize) = 0;

    void prepare();
    
protected:
    AudioProcessorBase(AudioProcessorType type);
    virtual ~AudioProcessorBase();
    
    int m_bufferSize;
    double m_sampleRate;
    
    AudioProcessorType m_type;
    
    bool m_isPrepared;

private:
    // ne pas appeller directement, utiliser prepare() !
    virtual void   internalPrepare() = 0;
    
};





#endif /* defined(__audioLib__AudioInternals__) */
