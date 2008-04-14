//*****************************************************************************
//**
//**   File:               SoundBuffer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************
#pragma once
#include <al/alut.h>
#include <string>

class SoundBuffer
{
private:
    ALuint  m_buffer;

public:
    SoundBuffer();
    ~SoundBuffer();

    const ALuint getBuffer() const { return m_buffer; }
    bool load( const std::string &filename );
};