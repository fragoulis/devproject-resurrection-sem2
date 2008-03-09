//*****************************************************************************
//**
//**   File:               SoundBuffer.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "SoundBuffer.h"
#include <al/alut.h>

SoundBuffer::SoundBuffer():
m_buffer(0) 
{}

SoundBuffer::~SoundBuffer()
{ 
    alDeleteBuffers(1, &m_buffer); 
}

bool SoundBuffer::load( const char *filename )
{
    // Load wav data into a buffer.
    m_buffer = alutCreateBufferFromFile( filename );

    if( m_buffer == AL_NONE ) return AL_FALSE;

    return AL_TRUE;
}