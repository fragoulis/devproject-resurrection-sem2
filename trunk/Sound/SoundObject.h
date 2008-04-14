#pragma once
#include "SoundSource.h"
class SoundBuffer;
class WorldObject;

class SoundObject
{
private:
    SoundSource m_source;
    WorldObject *m_object;
    SoundBuffer* m_sound;

public:
    SoundObject();
    SoundObject( WorldObject*, SoundBuffer* );

    void set( WorldObject*, SoundBuffer* );
    void update();

    void play() const        { m_source.play(); }
    void setPitch( float p ) { m_source.setPitch(p); }
    void setGain( float g )  { m_source.setGain(g); }
    void setLoop( bool l )   { m_source.setLoop(l); }
};