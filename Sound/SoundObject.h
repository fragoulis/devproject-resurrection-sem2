#pragma once
#include "SoundSource.h"
class Sound;
class WorldObject;

class SoundObject
{
private:
    SoundSource m_source;
    WorldObject *m_object;
    Sound* m_sound;

public:
    SoundObject();
    SoundObject( WorldObject*, Sound* );

    void set( WorldObject*, Sound* );
    void update();

    void play() const { m_source.play(); }

};