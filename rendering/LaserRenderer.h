#ifndef _RES_LASER_RENDERER_H_
#define _RES_LASER_RENDERER_H_
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../math/Vector4.h"

#include <vector>


/**
 * Renders lasers.
 *
 */

class Graphics;
class Texture;
class LaserRenderer : public EventListener< Laser_Spawned >,
						public EventListener< Laser_Despawned >,
						public EventListener< Level_Unload >
					 
{
public:
	LaserRenderer();
	virtual ~LaserRenderer();

	void render(Graphics& g) const;

	void onEvent(Laser_Spawned&);
	void onEvent(Laser_Despawned&);
	void onEvent(Level_Unload&);

	void update(const float dt);

private:

	struct LaserInfo_t
	{
		const Laser * laser;
		float timeElapsed;
		LaserInfo_t(const Laser * l):laser(l),timeElapsed(0.0f){}
	};

	int m_laserTypePos,
		m_laserTypeNeg;

	Vector4 m_posColor,
			m_negColor;

	Texture * m_flare;

	std::vector<LaserInfo_t> m_lasers;
};


#endif