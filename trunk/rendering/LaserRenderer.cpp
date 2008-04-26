#include "LaserRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../math/Vector3.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../gfxutils/Misc/Logger.h"
#include "../Gamelogic/GameLogic.h"

using namespace std;

static const float LASER_SCALE = 5.0f;

LaserRenderer :: LaserRenderer()
{
	EventManager::instance().registerEventListener< Laser_Spawned >(this);
	EventManager::instance().registerEventListener< Laser_Despawned >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);

	m_laserTypePos = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerPositive");
	m_laserTypeNeg = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerNegative");

	m_laserTex = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getLaserTextureName());
	m_noiseTex = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getNoiseTexture());

	// FIXME : editable colors?
	m_posColor = Vector4(1.0f,0.5f,0.5f,1.0f);
	m_negColor = Vector4(0.5f,1.0f,0.5f,1.0f);
}

LaserRenderer :: ~LaserRenderer()
{
	EventManager::instance().unRegisterEventListener< Laser_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Laser_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Level_Unload >(this);
}


void LaserRenderer :: onEvent(Level_Unload&)
{
	m_lasers.clear();
}

void LaserRenderer :: render(Graphics& g) const
{
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	ShaderManager::instance()->begin("laserShader2");
	m_laserTex->bind();
	ShaderManager::instance()->setUniform1i("flareTex",0);

	// New version adds the below lines , till statics
	m_noiseTex->bind(1);
	ShaderManager::instance()->setUniform1i("Noise",1);
	
	ShaderManager::instance()->setUniform1f("glowFallOff",0.024f);
	ShaderManager::instance()->setUniform1f("speed",1.86f);
	ShaderManager::instance()->setUniform1f("sampleDist",0.0076f);
	ShaderManager::instance()->setUniform1f("ambientGlow",0.5f);
	ShaderManager::instance()->setUniform1f("ambientGlowHeightScale",1.68f);
	ShaderManager::instance()->setUniform1f("vertNoise",0.78f);
	ShaderManager::instance()->setUniform1f("glowStrength",144.0f);
	ShaderManager::instance()->setUniform1f("height",0.44f);

	static const Vector2 lltex(0.0f,0.0f);
	static const Vector2 texext(1.0f,1.0f);

	CKLOG(string("Visible lasers : ") + ToString<unsigned>(unsigned(m_lasers.size())),2);

	for(std::vector<LaserInfo_t>::const_iterator it = m_lasers.begin();
		it != m_lasers.end();
		++it)
	{
		const Laser * laser = it->laser;

		// New Version

		const float w = laser->getWidth();
		Vector3 right = Vector3::cross(laser->getDirection(),Vector3(0.0f,1.0f,0.0f));
		right *= w*LASER_SCALE;

		const Vector3 up(LASER_SCALE*(laser->getFrontPoint() - laser->getBackPoint()));
		const Vector3 ll(0.5f*(laser->getBackPoint().getVector()+laser->getFrontPoint().getVector()) 
						 - right*0.5f - up*0.5f);

		if (laser->getType() == m_laserTypePos)
			ShaderManager::instance()->setUniform4fv("color",m_posColor.cfp());
		else
			ShaderManager::instance()->setUniform4fv("color",m_negColor.cfp());

		ShaderManager::instance()->setUniform1f("timeElapsed",it->timeElapsed);		

		RenderEngine::drawTexturedQuad(ll,right,up,lltex,texext);
	}
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void LaserRenderer :: onEvent(Laser_Spawned& evt)
{
	m_lasers.push_back(LaserInfo_t(evt.getValue()));
}

void LaserRenderer :: onEvent(Laser_Despawned& evt)
{
	const Laser * laser = evt.getValue();
	for(std::vector<LaserInfo_t>::iterator it = m_lasers.begin();
		it != m_lasers.end();
		++it)
	{
		if(it->laser == laser)
		{
			m_lasers.erase(it);
			break;
		}
	}
}

void LaserRenderer :: update(const float dt)
{
	Point3 pts[4];
	RenderEngine::instance().getWsScreenEdges(pts,GameLogic::instance().getGamePlaneHeight());

	int vp[4];
	RenderEngine::instance().getViewport(vp);
	for(std::vector<LaserInfo_t>::iterator it = m_lasers.begin();
		it != m_lasers.end();)
	{
		it->timeElapsed += dt;
		
		// compute low center of laser
		const Laser * laser = it->laser;
		const Vector3 up(LASER_SCALE*(laser->getFrontPoint() - laser->getBackPoint()));
		const Vector3 lc(0.5f*(laser->getBackPoint().getVector()+laser->getFrontPoint().getVector()) - up*0.5f);
		
		// get it's screen position (viewport = screen)
		Point3 pt = RenderEngine::instance().getScreenPositionFromMapPosition(Point3(lc));

		// check for containment
		if(((pt.getX() < vp[0]) || (pt.getX() > (vp[0] + vp[2]))) ||
			((pt.getY() < vp[1]) || (pt.getY() > (vp[1] + vp[3]))))
			it = m_lasers.erase(it);
		else
			++it;
	}
}