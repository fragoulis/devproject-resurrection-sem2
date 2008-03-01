#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
class ParserSection;

class AIEngine :
	public Singleton< AIEngine >,
	public EventListener< Level_Load >,
	public EventListener< Level_Unload >,
	public EventListener< Player_Spawned >,
	public EventListener< Enemy_Spawned >,
	public EventListener< Player_Destroyed >
{
public:

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void onEvent(Level_Load&);
	void onEvent(Level_Unload&);
	void onEvent(Player_Spawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Player_Destroyed&);

	void update(float dt);

private:

	Playership* m_playership;


	friend Singleton< AIEngine >;
	AIEngine();
	virtual ~AIEngine();
};
