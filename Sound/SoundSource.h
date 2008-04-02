//*****************************************************************************
//**
//**   File:               SoundSource.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************
#pragma once
#include <al/al.h>
#include "../math/Point3.h"

class SoundSource
{
private:
    ALuint  m_source;
    float   m_pitch;
    float   m_gain;
    Point3  m_position;
    Point3  m_velocity;
    bool    m_loop;
    bool    m_playing;

public:
    SoundSource();
    ~SoundSource();

    bool init(const ALuint * const buffer, const ALuint count = 1);
    bool setBuffer(const ALuint * const buffer, const ALuint count = 1);
    void play();
    void stop();
    void pause();
    void toggle();

    bool isPlaying() const { return m_playing; }

    // Mutators
    void setLoop( const bool loop );
    void setPitch( const float pitch );
    void setGain( const float gain );
    void setPosition( const Point3& );
    void setVelocity( const Point3& );
};