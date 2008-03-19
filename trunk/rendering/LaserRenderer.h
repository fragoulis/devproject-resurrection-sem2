#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../math/Vector4.h"

#include <vector>

class Graphics;
class Texture;
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

	Vector4 m_posColor,
			m_negColor;

	Texture * m_laserTex;

	std::vector<Laser *> m_lasers;
};