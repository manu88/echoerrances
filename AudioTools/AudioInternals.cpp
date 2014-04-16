//
//  AudioInternals.cpp
//  audioLib
//
//  Created by Manuel Deneu on 17/03/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "AudioInternals.h"
#include "Debug_pd.h"




/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
AudioNode::AudioNode(AudioProcessorBase *proc , int nodeId):
m_nodeId(nodeId),
m_ownedProcessor(proc),
m_from(nullptr),
m_to(nullptr)
{
    
}

AudioNode::~AudioNode()
{
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int AudioGraph::s_idCounter = 0;


AudioGraph::AudioGraph()
{
    
}

AudioGraph::~AudioGraph()
{

}



AudioNode* AudioGraph::getNodeForId(int nodeId)
{
    for(std::list<AudioNode*>::iterator iter = m_nodesList.begin();iter != m_nodesList.end(); iter++)
    {
        if ((*iter)->getNodeId() == nodeId)
        {
            return *iter;
        }
        
    }
    return nullptr;
    
    
}

int AudioGraph::addNode(AudioProcessorBase* procToAdd)
{
    m_nodesList.push_back(new AudioNode(procToAdd , getNextId()) );
    post("new Node added %i",s_idCounter);
    
    return s_idCounter;
}


bool AudioGraph::removeNode(int nodeIdToRemove)
{
    
    for(std::list<AudioNode*>::iterator iter = m_nodesList.begin();iter != m_nodesList.end(); iter++)
    {
        if ((*iter)->getNodeId() == nodeIdToRemove)
        {
            post("node %i removed",(*iter)->getNodeId());
            m_nodesList.remove( (*iter));
            
            return true;
        }

    }
    return false;
}


bool AudioGraph::connect(int sourceNode , int destNode)
{
    AudioNode *source = nullptr;
    AudioNode *dest = nullptr;
    
    
    for(std::list<AudioNode*>::iterator iter = m_nodesList.begin();iter != m_nodesList.end(); iter++)
    {
        if ((*iter)->getNodeId() == sourceNode)
            source = (*iter);
        
        else if ((*iter)->getNodeId() == destNode)
            dest = (*iter);
        
    }
    
    if (source && dest)
    {
        source->setDestinatation(dest);
        dest->setSource(source);
        
        return true;
    }
    

    return false;
    
    
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

AudioProcessorBase::AudioProcessorBase(AudioProcessorType type):
m_bufferSize(64),
m_sampleRate(44.100),
m_type(type),
m_isPrepared(false)
{
    
}

AudioProcessorBase::~AudioProcessorBase()
{
    
}

void AudioProcessorBase::setConfig(int bufferSize, double sampleRate)
{
    m_isPrepared = false;
    setSampleRate(sampleRate);
    setBufferSize(bufferSize);
}

void AudioProcessorBase::prepare()
{
    pdAssert(!m_isPrepared, "AudioProc is set prepared but is not");
        
    internalPrepare();
    m_isPrepared = true;
}




/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    EMPTY IMPLEMENTATIONS
 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
void AudioProcessorBase::internalPrepare()
{
    pdAssert(false, "base internalPrepare() called!");    
}


inline void AudioProcessorBase::process(float **ins, float **outs, int bufferSize)
{
    pdAssert(false, "base process() called!");
}