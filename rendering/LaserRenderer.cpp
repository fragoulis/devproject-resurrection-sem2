#include "LaserRenderer.h"
#include "RenderEngine.h"
//#include "../gfx/Shaders/ShaderManager.h"
//#include "../gfx/Texture/Texture.h"
#include "../math/Vector3.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../gfxutils/Misc/Logger.h"
#include "../Gamelogic/GameLogic.h"
#include "../gfxutils/texture/texture.h"
#include "../gfxutils/texture/textureMgr.h"

using namespace std;

static const float LASER_SCALE = 5.0f;

LaserRenderer :: LaserRenderer()
{
	EventManager::instance().registerEventListener< Laser_Spawned >(this);
	EventManager::instance().registerEventListener< Laser_Despawned >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);

	m_laserTypePos = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerPositive");
	m_laserTypeNeg = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerNegative");

	//m_laserTex = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getLaserTextureName());
	//m_noiseTex = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getNoiseTexture());

	// FIXME : editable colors?
	m_posColor = Vector4(1.0f,0.5f,0.5f,1.0f);
	m_negColor = Vector4(0.5f,1.0f,0.5f,1.0f);

	//TplPalette * pal = TextureMgr::instance().loadPalette("flare.tpl","flareTPL.txt");
	//m_flare = new Texture(pal,0,"flare");
}

LaserRenderer :: ~LaserRenderer()
{
	EventManager::instance().unRegisterEventListener< Laser_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Laser_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Level_Unload >(this);
	//delete m_flare;
}


void LaserRenderer :: onEvent(Level_Unload&)
{
	m_lasers.clear();
}

void LaserRenderer :: render(Graphics& g) const
{
	return;
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

		/*
		if (laser->getType() == m_laserTypePos)
			ShaderManager::instance()->setUniform4fv("color",m_posColor.cfp());
		else
			ShaderManager::instance()->setUniform4fv("color",m_negColor.cfp());
			*/

		//RenderEngine::drawTexturedQuad(ll,right,up,lltex,texext);
	}
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
	//Point3 pts[4];
	//RenderEngine::instance().getWsScreenEdges(pts,GameLogic::instance().getGamePlaneHeight());

	//int vp[4];
	//RenderEngine::instance().getViewport(vp);
	//for(std::vector<LaserInfo_t>::iterator it = m_lasers.begin();
	//	it != m_lasers.end();)
	//{
	//	it->timeElapsed += dt;
	//	
	//	// compute low center of laser
	//	const Laser * laser = it->laser;
	//	const Vector3 up(LASER_SCALE*(laser->getFrontPoint() - laser->getBackPoint()));
	//	const Vector3 lc(0.5f*(laser->getBackPoint().getVector()+laser->getFrontPoint().getVector()) - up*0.5f);
	//	
	//	// get it's screen position (viewport = screen)
	//	Point3 pt = RenderEngine::instance().getScreenPositionFromMapPosition(Point3(lc));

	//	// check for containment
	//	if(((pt.getX() < vp[0]) || (pt.getX() > (vp[0] + vp[2]))) ||
	//		((pt.getY() < vp[1]) || (pt.getY() > (vp[1] + vp[3]))))
	//		it = m_lasers.erase(it);
	//	else
	//		++it;
	//}
}