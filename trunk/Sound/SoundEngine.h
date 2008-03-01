#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
class ParserSection;

class SoundEngine :
	public Singleton< SoundEngine >,
	public EventListener< Player_Destroyed >,
	public EventListener< Player_Drained >,
	public EventListener< Level_Load >
{
public:

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void onEvent(Level_Load&);
	void onEvent(Player_Destroyed&);
	void onEvent(Player_Drained&);

private:

	friend Singleton< SoundEngine >;
	SoundEngine();
	virtual ~SoundEngine();
};
