
#ifndef __encoder___WavLoader__
#define __encoder___WavLoader__

#include <iostream>

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;			// sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;			// subchunk2_size denotes the number of samples.
} header;

typedef struct header_file* header_p;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Structure de lecture générique d'un fichier audio
 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

class WavLoader
{
public:
    
    enum { maxIntValue = 32768 };
    
    WavLoader();
    ~WavLoader();

    bool getWavAttributes(const char* file);
    
    bool openAndCopyWavFile(const char* filepath, float*bufferToFill)
    {
        if (!m_fileExists)
            getWavAttributes(filepath);
        
        return readFile(filepath, bufferToFill);
        
    }
    
    int getSampleRate() const
    {
        return m_sampleRate;
    }
    
    int getNumSamples() const
    {
        return m_numSamples;
    }
    
    int getNumChannels() const
    {
        return m_channels;
    }
    
    
protected:
    bool readFile(const char* file, float* bufferToFill);
    void closeFile();
    
    
    int      m_sampleRate;
    int      m_numSamples;
    int      m_channels;
    
    
    FILE*    m_currentFile;

private:
    bool m_fileExists;
    
};

#endif /* defined(__encoder___WavLoader__) */
