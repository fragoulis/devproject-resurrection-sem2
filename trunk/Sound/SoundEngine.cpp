#include "SoundEngine.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/EnergyTypes.h"

SoundEngine :: SoundEngine()
{
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_Drained >(this);
}

SoundEngine :: ~SoundEngine()
{
}


void SoundEngine :: onEvent(Player_Destroyed& pd)
{
	Playership* ps = pd.getPointer();
	EnergyType type = pd.getValue();

	// TODO: play sound effect for playership explosion
}

void SoundEngine :: onEvent(Player_Drained& pd)
{
	Playership* ps = pd.getPointer();
	EnergyType type = pd.getValue1();
	int amount = pd.getValue2();

	// TODO: play sound effect for energy draining from playership
}
