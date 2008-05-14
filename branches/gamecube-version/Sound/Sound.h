#pragma once
#include "SoundSource.h"
class SoundBuffer;
class Point3;

class Sound
{
private:
    SoundSource m_source;
    SoundBuffer* m_buffer;

public:
    Sound();
    Sound( SoundBuffer* );

    unsigned getSource() const { return m_source.GetSource(); }
    unsigned getBuffer() const;

    void set( SoundBuffer* );
    void update( const Point3& );

    void play() const        { m_source.play(); }
    void stop() const        { m_source.stop(); }
    void setPitch( float p ) { m_source.setPitch(p); }
    void setGain( float g )  { m_source.setGain(g); }
    void setLoop( bool l )   { m_source.setLoop(l); }
    bool isPlaying() const   { return m_source.isPlaying(); }
};