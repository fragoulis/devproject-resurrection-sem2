#include "Sound.h"
#include "SoundBuffer.h"

Sound::Sound():
m_buffer(0)
{}

Sound::Sound( SoundBuffer *sound ):
m_buffer(0)
{
    set(sound);
}

void Sound::set( SoundBuffer *sound )
{
    m_buffer = sound;

    const unsigned b = m_buffer->getBuffer();
    m_source.init(&b);
}

// Updates the position of the sound source by copying the 
// position of the connected object
void Sound::update( const Point3 &position )
{
    m_source.setPosition( position );
}

unsigned Sound::getBuffer() const { return m_buffer->getBuffer(); }