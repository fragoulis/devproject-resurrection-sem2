//*****************************************************************************
//**
//**   File:               SoundBuffer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#ifndef __SOUNDBUFFER_H__
#define __SOUNDBUFFER_H__
#pragma once

class SoundBuffer
{
private:
    typedef unsigned int ALuint;
    ALuint  m_buffer;

public:
    SoundBuffer();
    ~SoundBuffer();

    const ALuint getBuffer() const { return m_buffer; }
    bool load( const char *filename );
};

#endif // __SOUNDBUFFER_H__