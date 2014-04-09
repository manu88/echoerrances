//
//  WavPlayer.cpp
//  encoder~
//
//  Created by Manuel Deneu on 31/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include "WavLoader.h"
#include "FloatComputation.h"
#include "AudioTools.h"

#include "Debug_pd.h"




WavLoader::WavLoader():
    m_sampleRate(0),
    m_numSamples(0),
    m_channels(0),
    m_currentFile(NULL),
    m_fileExists(false)
{
    
}

WavLoader::~WavLoader()
{
    // closeFile() ??
}

void WavLoader::closeFile()
{
    fclose(m_currentFile);
    m_currentFile = NULL;
}


bool WavLoader::getWavAttributes(const char* file)
{
    bool result = false;
    FILE * infile = fopen(file,"rb");
	
    

	header_p meta = (header_p)malloc(sizeof(header));
    

	if (infile)
	{
		fread(meta, 1, sizeof(header), infile);
        

        m_sampleRate = meta->sample_rate;
        m_numSamples = meta->subchunk2_size;
        m_channels   = meta->num_channels;
        
        
        
        result = true;
	}
    else
    {
        //PB...
    }
    
    free(meta);
    closeFile();
    
    m_fileExists = result;
    
    return result;

}


bool WavLoader::readFile(const char* file, float* bufferToFill)
{
    bool result = false;
    FILE * infile = fopen(file,"rb");
	
    
	int BUFSIZE = 512;

    
	short buff[BUFSIZE];
	header_p meta = (header_p)malloc(sizeof(header));
    
	int nb;
	if (infile)
	{
		fread(meta, 1, sizeof(header), infile);
        
        if (meta->sample_rate!=m_sampleRate)
        {
            free(meta);
            closeFile();
            return false;
        }
        
        m_numSamples = meta->subchunk2_size;
        
        int index = 0;
		while (!feof(infile))
		{
			nb = fread(buff,meta->block_align,BUFSIZE,infile);

            for (int i=0;i<nb;i++)
                bufferToFill[index+i]  = (((float)buff[i])/ maxIntValue);

            index += nb;
		}
        

        
        
        result = true;
	}
    else
    {
        //PB...
    }
    
    free(meta);
    closeFile();
    
    return result;
}