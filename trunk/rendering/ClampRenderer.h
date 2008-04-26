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
class ClampRenderer : public EventListener<Enemy_Attached_To_Player>,
					   public EventListener<Enemy_Destroyed>,
					   public EventListener<Enemy_Despawned>
{
public :
	ClampRenderer();
	~ClampRenderer();

	void onEvent(Enemy_Attached_To_Player&);
	void onEvent(Enemy_Destroyed&);
	void onEvent(Enemy_Despawned&);

	void render(Graphics& g) const;
	void update(const float dt);

private :

	const Playership * m_plship;
	std::vector<const Enemyship *> m_enships;
	float m_accumTimer;
};