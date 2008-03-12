#include "LaserRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../math/Vector3.h"
#include "../GameLogic/WorldObjectTypeManager.h"

using namespace std;

LaserRenderer :: LaserRenderer()
{
	EventManager::instance().registerEventListener< Laser_Spawned >(this);
	EventManager::instance().registerEventListener< Laser_Despawned >(this);

	m_laserTypePos = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerPositive");
	m_laserTypeNeg = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerNegative");
}

void LaserRenderer :: render(Graphics& g) const
{
	// FIXME : apply shader & get also the color? neg / pos ray?
	// Apply laser shader
	//ShaderManager::instance()->begin("LaserShader");

	for(std::vector<Laser *>::const_iterator it = m_lasers.begin();
		it != m_lasers.end();
		++it)
	{
		Laser * laser = (*it);

		const float w = laser->getWidth();
		Vector3 right = Vector3::cross(laser->getDirection(),Vector3(0.0f,1.0f,0.0f));
		right *= w;
		const Point3 ll(laser->getBackPoint() - right*0.5f);
		const Vector3 up(laser->getFrontPoint() - laser->getBackPoint());

		// We can use the (*it)->getType() here to determine the color that we'll use for the laser

		// for now draw a quad
		RenderEngine::drawQuad(ll.getVector(),right,up);
	}
}

void LaserRenderer :: onEvent(Laser_Spawned& evt)
{
	m_lasers.push_back(evt.getValue());
}

void LaserRenderer :: onEvent(Laser_Despawned& evt)
{
	const Laser * laser = evt.getValue();
	for(std::vector<Laser *>::iterator it = m_lasers.begin();
		it != m_lasers.end();
		++it)
	{
		if((*it) == laser)
		{
			m_lasers.erase(it);
			break;
		}
	}
}