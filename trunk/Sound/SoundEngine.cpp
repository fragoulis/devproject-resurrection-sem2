//*****************************************************************************
//**
//**   File:               SoundEngine.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "SoundEngine.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"

SoundEngine :: SoundEngine()
{
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_EnergyDrained >(this);
}

SoundEngine :: ~SoundEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void SoundEngine :: onApplicationLoad(const ParserSection& ps)
{
	// TODO: read ParserSection for info and load sounds from disk
}

void SoundEngine :: onApplicationUnload()
{
	// TODO: delete allocated memory
}

void SoundEngine :: onEvent(Level_Load& ll)
{
	const ParserSection* ps = ll.getValue1();

	// TODO: make sure all sounds required for this level are loaded
	// If not, load them now.
}

void SoundEngine :: onEvent(Player_Destroyed& pd)
{
	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();

	// TODO: play sound effect for playership explosion
}

void SoundEngine :: onEvent(Player_EnergyDrained& pd)
{
	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();
	int amount = pd.getValue3();

	// TODO: play sound effect for energy draining from playership
}
