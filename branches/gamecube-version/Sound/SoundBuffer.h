#pragma once
#include <al/alut.h>
#include <string>

class SoundBuffer
{
private:
    ALuint  m_buffer;

public:
    SoundBuffer();
    ~SoundBuffer();

    ALuint getBuffer() const { return m_buffer; }
    bool load( const std::string &filename );
};