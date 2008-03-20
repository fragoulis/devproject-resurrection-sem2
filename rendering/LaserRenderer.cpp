#include "LaserRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../math/Vector3.h"
#include "../GameLogic/WorldObjectTypeManager.h"

using namespace std;

LaserRenderer :: LaserRenderer()
{
	EventManager::instance().registerEventListener< Laser_Spawned >(this);
	EventManager::instance().registerEventListener< Laser_Despawned >(this);

	m_laserTypePos = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerPositive");
	m_laserTypeNeg = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerNegative");

	m_laserTex = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getLaserTextureName());

	// FIXME : editable colors?
	m_posColor = Vector4(1.0f,0.0f,0.0f,1.0f);
	m_negColor = Vector4(0.0f,1.0f,0.0f,1.0f);
}

void LaserRenderer :: render(Graphics& g) const
{
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	ShaderManager::instance()->begin("laserShader");
	m_laserTex->bind();
	ShaderManager::instance()->setUniform1i("flareTex",0);

	static const Vector2 lltex(0.0f,0.0f);
	static const Vector2 texext(1.0f,1.0f);

	for(std::vector<Laser *>::const_iterator it = m_lasers.begin();
		it != m_lasers.end();
		++it)
	{
		Laser * laser = (*it);

		const float w = laser->getWidth();
		Vector3 right = Vector3::cross(laser->getDirection(),Vector3(0.0f,1.0f,0.0f));
		right *= w;
		//const Point3 ll((laser->getBackPoint() - right*0.5f) - laser->getDirection()*115.0f); - COOL
		//const Vector3 up((laser->getFrontPoint() - laser->getBackPoint())*12.0f);

		const Point3 ll(laser->getBackPoint() - right*0.5f);
		const Vector3 up(laser->getFrontPoint() - laser->getBackPoint());

		// We can use the (*it)->getType() here to determine the color that we'll use for the laser
		if ((*it)->getType() == m_laserTypePos)
			ShaderManager::instance()->setUniform4fv("color",m_posColor.cfp());
		else
			ShaderManager::instance()->setUniform4fv("color",m_negColor.cfp());

		// for now draw a quad
		RenderEngine::drawTexturedQuad(ll.getVector(),right,up,lltex,texext);
	}
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
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