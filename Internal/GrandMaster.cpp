//
//  GrandMaster.cpp
//  audioLib
//
//  Created by Manuel Deneu on 14/04/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "GrandMaster.h"

#include "Debug_pd.h"


GrandMaster* GrandMaster::s_instance = nullptr;


GrandMaster::GrandMaster()
{
    post("new grand master");
    m_graph = new AudioGraph();
    
}


GrandMaster::~GrandMaster()
{
    post("delete grand master");
    delete m_graph;
}