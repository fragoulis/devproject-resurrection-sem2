//*****************************************************************************
//**
//**   File:               SoundManager.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__
#pragma once
#pragma comment (lib, "openal32")
#pragma comment (lib, "alut")
#include "../../utility/Singleton.h"
#include <vector>
#include <map>

namespace game
{
namespace sound
{
    
    class SoundBuffer;
    class SoundSource;

/**
 * @class SoundManager
 * 
 * Manages sound clips/effects and music.
 * SoundManager is responsible for loading and playign all necessary sounds
 * and music.
 * Before being used it must be initialized as such: SoundMamanger::_Instance().init(); 
 */
class SoundManager// : public Singleton
{
public:
    void init();
    void play();

private:
    SoundManager();

private:
    typedef std::vector<SoundBuffer*> BufferList;
    //typedef std::map<SoundID,SoundBuffer*> BufferList;
    //typedef vector<SoundSource*> SourceList;

    BufferList m_buffers;
};

} // end namespace sound
} // end namespace game

#endif // __SOUNDMANAGER_H__