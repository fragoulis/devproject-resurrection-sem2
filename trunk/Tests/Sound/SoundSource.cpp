//*****************************************************************************
//**
//**   File:               SoundSource.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "SoundSource.h"
#include <al/al.h>

SoundSource::SoundSource():
m_source(0),
m_playing(false),
m_loop(false),
m_pitch(1.0f),
m_gain(1.0f) 
{}

SoundSource::~SoundSource() 
{ 
    alDeleteSources(1, &m_source); 
}

bool SoundSource::init( const ALuint * const buffer, 
                        const ALuint count )
{
    if( !buffer )
        return AL_FALSE;

    alGetError();

    // Bind buffer with a source.
    alGenSources(1, &m_source);

    if(alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    if( count == 1 )
        alSourcei ( m_source, AL_BUFFER, buffer[0] );
    else
        alSourceQueueBuffers( m_source, count, buffer );

    alSourcef ( m_source, AL_PITCH,    m_pitch  );
    alSourcef ( m_source, AL_GAIN,     m_gain   );
    alSourcefv( m_source, AL_POSITION, m_pos    );
    alSourcefv( m_source, AL_VELOCITY, m_vel    );
    alSourcei ( m_source, AL_LOOPING,  m_loop   ); 

    // Do another error check and return.
    if(alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    return AL_TRUE;
}

void SoundSource::play()   
{ 
    alSourcePlay(m_source);
    m_playing = true;
}

void SoundSource::stop()
{ 
    alSourceStop(m_source);
    m_playing = false;
}

void SoundSource::pause() 
{
    alSourcePause(m_source);
    m_playing = false;
}

void SoundSource::toggle()
{
    if( m_playing ) pause();
    else
        play();
}

void SoundSource::setLoop( const bool loop )
{
    alSourcei( m_source, AL_LOOPING, loop );
    m_loop = loop;
}

void SoundSource::setPitch( const float pitch )
{
    alSourcef( m_source, AL_PITCH, pitch  );
    m_pitch = pitch;
}

void SoundSource::setGain( const float gain )
{
    alSourcef( m_source, AL_GAIN, m_gain );
    m_gain = gain;
}

void SoundSource::setPosition( const float * const pos )
{
    alSourcefv( m_source, AL_POSITION, pos );
    m_pos[0] = pos[0]; 
    m_pos[1] = pos[1]; 
    m_pos[2] = pos[2];
}

void SoundSource::setVelocity( const float * const vel )
{
    alSourcefv( m_source, AL_VELOCITY, vel );
    m_vel[0] = vel[0];
    m_vel[1] = vel[1];
    m_vel[2] = vel[2];
}