#include "SoundSource.h"

SoundSource::SoundSource():
m_source(0),
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

    // Bind buffer with a source
    // If already initialized, just switch buffers internally
    if( m_source > 0 ) return setBuffer( buffer, count );

    alGenSources(1, &m_source);

    if(alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    setBuffer( buffer, count );

    alSourcef ( m_source, AL_PITCH,    m_pitch );
    alSourcef ( m_source, AL_GAIN,     m_gain );
    alSourcefv( m_source, AL_POSITION, m_position.cfp() );
    alSourcefv( m_source, AL_VELOCITY, m_velocity.cfp() );
    alSourcei ( m_source, AL_LOOPING,  m_loop ); 

    // Do another error check and return.
    if(alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    return AL_TRUE;
}

bool SoundSource::setBuffer( const ALuint * const buffer, 
                             const ALuint count )
{
    if( !buffer )
        return AL_FALSE;

    if( count == 1 )
        alSourcei ( m_source, AL_BUFFER, buffer[0] );
    else
        alSourceQueueBuffers( m_source, count, buffer );

    return AL_TRUE;
}

bool SoundSource::isPlaying() const
{
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

void SoundSource::play() const
{ 
    alSourcePlay(m_source);
}

void SoundSource::stop() const
{ 
    alSourceStop(m_source);
}

void SoundSource::pause() const
{
    alSourcePause(m_source);
}

void SoundSource::toggle() const
{
    if( isPlaying() ) pause();
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

void SoundSource::setPosition( const Point3& pos )
{
    alSourcefv( m_source, AL_POSITION, pos.cfp() );
    m_position = pos;
}

void SoundSource::setVelocity( const Point3& vel )
{
    alSourcefv( m_source, AL_VELOCITY, vel.cfp() );
    m_velocity = vel;
}