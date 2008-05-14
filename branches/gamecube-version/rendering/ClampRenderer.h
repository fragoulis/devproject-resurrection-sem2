#pragma once

#include <vector>

#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"

/*
	Class which renders the attachments of the playership to the enemy ships
*/

class Playership;
class Enemyship;
class Graphics;
class ClampRenderer : public EventListener<Interceptor_Clamped>,
					   public EventListener<Enemy_Destroyed>,
					   public EventListener<Enemy_Despawned>,
					   public EventListener<Player_Spawned>,
					   public EventListener<Player_Respawned>,
					   public EventListener<Player_Destroyed>,
					   public EventListener<Player_Despawned>
{
public :
	ClampRenderer();
	~ClampRenderer();

	void onEvent(Interceptor_Clamped&);
	void onEvent(Enemy_Destroyed&);
	void onEvent(Enemy_Despawned&);
	void onEvent(Player_Spawned&);
	void onEvent(Player_Respawned&);
	void onEvent(Player_Destroyed&);
	void onEvent(Player_Despawned&);

	void render(Graphics& g) const;
	void update(const float dt);

private :

	const Playership * m_plship;
	std::vector<const Enemyship *> m_enships;
	float m_accumTimer;
};