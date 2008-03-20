//*****************************************************************************
//**
//**   File:               SoundEngine.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
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
