#include "SoundObject.h"
#include "SoundBuffer.h"
#include "../GameLogic/WorldObject.h"

SoundObject::SoundObject()
{}

SoundObject::SoundObject( WorldObject *object, SoundBuffer *sound )
{
    set(object, sound);
}

void SoundObject::set( WorldObject *object, SoundBuffer *sound )
{
    m_object = object;
    m_sound = sound;

    const unsigned b = m_sound->getBuffer();
    m_source.init(&b);
}

// Updates the position of the sound source by copying the 
// position of the connected object
void SoundObject::update()
{
    m_source.setPosition( m_object->getPosition() );
}