#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "../GameLogic/Lasers/Laser.h"

#include <vector>

class Graphics;

class LaserRenderer : public EventListener< Laser_Spawned >,
						public EventListener< Laser_Despawned >
					 
{
public:
	LaserRenderer();
	~LaserRenderer(){}

	void render(Graphics& g) const;

	void onEvent(Laser_Spawned&);
	void onEvent(Laser_Despawned&);

private:

	int m_laserTypePos,
		m_laserTypeNeg;

	std::vector<Laser *> m_lasers;
};