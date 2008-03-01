#include "SoundEngine.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"

SoundEngine :: SoundEngine()
{
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_Drained >(this);
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
	const ParserSection& ps = ll.getReference();

	// TODO: make sure all sounds required for this level are loaded
	// If not, load them now.
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
