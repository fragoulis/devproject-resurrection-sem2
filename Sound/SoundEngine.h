#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"

class SoundEngine :
	public Singleton< SoundEngine >,
	public EventListener< Player_Destroyed >,
	public EventListener< Player_Drained >
{
public:

	void onEvent(Player_Destroyed&);
	void onEvent(Player_Drained&);

private:

	friend Singleton< SoundEngine >;
	SoundEngine();
	virtual ~SoundEngine();
};
