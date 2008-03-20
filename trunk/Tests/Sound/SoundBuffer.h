//*****************************************************************************
//**
//**   File:               SoundBuffer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
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