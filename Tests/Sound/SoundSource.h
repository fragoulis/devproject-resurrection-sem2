//*****************************************************************************
//**
//**   File:               SoundSource.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#ifndef __SOUNDSOURCE_H__
#define __SOUNDSOURCE_H__
#pragma once

class SoundSource
{
private:
    typedef unsigned int ALuint;
    ALuint  m_source;
    float   m_pitch;
    float   m_gain;
    float   m_pos[3];
    float   m_vel[3];
    bool    m_loop;
    bool    m_playing;

public:
    SoundSource();
    ~SoundSource();

    bool init(const ALuint * const buffer, const ALuint count = 1);
    void play();
    void stop();
    void pause();
    void toggle();

    bool isPlaying() const { return m_playing; }

    // Mutators
    void setLoop( const bool loop );
    void setPitch( const float pitch );
    void setGain( const float gain );
    void setPosition( const float *pos );
    void setVelocity( const float *vel );
};

#endif // __SOUNDSOURCE_H__