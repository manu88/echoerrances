//
//  WavPlayer.cpp
//  encoder~
//
//  Created by Manuel Deneu on 31/01/14.
//  Copyright (c) 2014 Manuel Deneu. All rights reserved.
//

#include <cstdlib>

#include "WavLoader.h"
#include "FloatComputation.h"
#include "AudioTools.h"

#include "../PDObjects/Debug_pd.h"




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
    //! closeFile() ?? on sait jamais...
    closeFile();
}

void WavLoader::closeFile()
{
    if(m_currentFile)
        fclose(m_currentFile);
    m_currentFile = NULL;
}


bool WavLoader::getWavAttributes(const char* file)
{
    bool result = false;
    //FILE * infile = fopen(file,"rb");
    m_currentFile = fopen(file, "rb");



	header_p meta = (header_p)malloc(sizeof(header));


	//if (infile)
	if(m_currentFile)
	{
		//fread(meta, 1, sizeof(header), infile);
		fread(meta, 1, sizeof(header), m_currentFile);


        m_sampleRate = meta->sample_rate;
        m_numSamples = meta->subchunk2_size;
        m_channels   = meta->num_channels;



        result = true;
	}
    else
    {
        pdAssert(false, "Error in WavLoader::getWavAttributes");
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
    //FILE * infile = fopen(file,"rb");
    m_currentFile = fopen(file, "rb");


	const int bufSize = 512;


	short buff[bufSize];
	header_p meta = (header_p)malloc(sizeof(header));

	int nb;
	//if (infile)
      if(m_currentFile)
	{
		//fread(meta, 1, sizeof(header), infile);*
		fread(meta, 1, sizeof(header), m_currentFile);

        if (meta->sample_rate!=m_sampleRate)
        {
            free(meta);
            closeFile();
            return false;
        }

        m_numSamples = meta->subchunk2_size;

        int index = 0;
		//while (!feof(infile))
		while (!feof(m_currentFile))
		{
			//nb = fread(buff,meta->block_align,bufSize,infile);
			nb = fread(buff,meta->block_align,bufSize,m_currentFile);

            for (int i=0;i<nb;i++)
            {
                bufferToFill[index+i]  = (((float)buff[i])/ maxIntValue);

                pdAssert( ( ( bufferToFill[index+i] <=1.) && (bufferToFill[index+i] >=-1. ) ) , "val out of [-1;1] in HRTF 1");
            }

            index += nb;
		}



        result = true;
	}
    else
    {
        pdAssert(false, "Error in WavLoader::read");
    }

    free(meta);
    closeFile();

    return result;
}
