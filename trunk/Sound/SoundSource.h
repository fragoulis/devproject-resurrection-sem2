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

public:
    SoundSource();
    ~SoundSource();

    ALuint GetSource() const { return m_source; }

    bool init(const ALuint * const buffer, const ALuint count = 1);
    bool setBuffer(const ALuint * const buffer, const ALuint count = 1);
    void play() const;
    void stop() const;
    void pause() const;
    void toggle() const;
    bool isPlaying() const;

    // Mutators
    void setLoop( const bool loop );
    void setPitch( const float pitch );
    void setGain( const float gain );
    void setPosition( const Point3& );
    void setVelocity( const Point3& );
};