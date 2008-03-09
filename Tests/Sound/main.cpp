//*****************************************************************************
//**
//**   File:               main.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

//#include "SoundManager.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
//#include <al/al.h>
//#include <al/alc.h>
#include <al/alut.h>

#include <iostream>
using namespace std;

// Position of the source sound.
ALfloat SourcePos1[] = { -10.0f, -10.0f, 0.0 };
ALfloat SourcePos2[] = { 0.0, 10.0f, 0.0 };
ALfloat SourcePos3[] = { 10.0f, -10.0f, 0.0 };
// Velocity of the source sound.
ALfloat SourceVel1[] = { 0.0f, 0.0, 0.0 };
ALfloat SourceVel2[] = { 0.1f, 0.1f, 0.1f };
ALfloat SourceVel3[] = { 0.2f, 0.2f, 0.2f };


// Position of the listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
// Velocity of the listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };


void SetListenerValues()
{
    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
}

int main(int argc, char *argv[])
{
    // Initialize OpenAL and clear the error bit.
    alutInit(&argc, argv);
    alGetError();

    SetListenerValues();

    { 
#define LOAD_SOUND( snd, str ) if(!snd.load(str)) std::cout << "Couldnt load " << str << endl;

        SoundBuffer sound1, sound2, sound3;
        LOAD_SOUND(sound1, "ding.wav");
        LOAD_SOUND(sound2, "recycle.wav");
        LOAD_SOUND(sound3, "ringout.wav");

        unsigned buf;

        SoundSource src1, src2, src3;

        src1.setPosition( SourcePos1 );
        src1.setVelocity( SourceVel1 );
        buf = sound1.getBuffer();
        if(! src1.init(&buf) ) 
            std::cout << "Couldnt init sound on buffer " << buf << endl;

        src2.setPosition( SourcePos2 );
        src2.setVelocity( SourceVel2 );
        buf = sound2.getBuffer();
        if(! src2.init(&buf) ) 
            std::cout << "Couldnt init sound on buffer " << buf << endl;

        src3.setPosition( SourcePos3 );
        src3.setVelocity( SourceVel3 );
        unsigned bufs[3] = { sound1.getBuffer(), sound3.getBuffer(), sound2.getBuffer() };
        if(! src3.init(bufs,3) ) 
            std::cout << "Couldnt init sound on buffers" << endl;
        
        while(1)
        { 
            char c; cin >> c;
            switch(c)
            {
            case 'a': src1.toggle(); break;
            case 's': src2.toggle(); break;
            case 'd': src3.toggle(); break;
            case 'w': src3.setLoop(true); break;
            case 'e': src3.setLoop(false); break;
            case 'q': goto exit; break;
            }
        }
    }

exit:
    alutExit();
    return 0;
}
