//*****************************************************************************
//**
//**   File:               SoundEngine.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
class ParserSection;


/**
 * Listens for events and plays sounds when things happen
 */
class SoundEngine :
	public Singleton< SoundEngine >,
	public EventListener< Player_Destroyed >,
	public EventListener< Player_EnergyDrained >,
	public EventListener< Level_Load >
{
public:

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void onEvent(Level_Load&);
	void onEvent(Player_Destroyed&);
	void onEvent(Player_EnergyDrained&);

private:

	friend Singleton< SoundEngine >;
	SoundEngine();
	virtual ~SoundEngine();
};
