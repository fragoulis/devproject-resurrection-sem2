#include "SoundBuffer.h"

SoundBuffer::SoundBuffer():
m_buffer(0) 
{}

SoundBuffer::~SoundBuffer()
{ 
    alDeleteBuffers(1, &m_buffer); 
}

bool SoundBuffer::load( const std::string &filename )
{
    // Load wav data into a buffer.
    m_buffer = alutCreateBufferFromFile( filename.c_str() );

    if( m_buffer == AL_NONE ) return AL_FALSE;

    return AL_TRUE;
}