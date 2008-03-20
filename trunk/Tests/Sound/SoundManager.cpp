//*****************************************************************************
//**
//**   File:               SoundManager.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "SoundManager.h"
using namespace game::sound;

SoundManager::SoundManager()
{}

void SoundManager::init()
{
    // 1. Read how many sounds we are going to load

    // 2. Allocate array of N buffers
    // [if we are using std::map<enum,sound> to load sounds then better allocate with freelist]

    // 3. Initialize buffer objects
}