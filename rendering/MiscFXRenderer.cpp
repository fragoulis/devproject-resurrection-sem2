#include "MiscFXRenderer.h"
#include "../utility/RandomGenerator.h"
#include "../GameLogic/Objects/Playership.h"
#include "RenderEngine.h"
#include "../gfx/Camera.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Shaders/ShaderMAnager.h"
#include "../gfx/Texture/TextureIO.h"
#include <gl/glee.h>

static float RAY_MAX_LENGTH = 500.0f;
static float RAY_MAX_WIDTH = 80.0f;

MiscFXRenderer :: MiscFXRenderer()
:m_accumTime(0.0f),
m_godModeON(false),
m_plship(NULL),
m_camera(NULL)
{
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Respawned >(this);
	// generate the rays
	for(int i=0;i<RAY_NUM;++i)
	{
		// get a vector
		RandomGenerator::GET_RANDOM_VECTOR3(m_rays[i],Vector3(-1.0f,-1.0f,-1.0f),Vector3(1.0f,1.0f,1.0f));
		m_rays[i].normalize();
	}
	m_rayTex = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getLaserTextureName());
	
}

MiscFXRenderer :: ~MiscFXRenderer()
{

}

void MiscFXRenderer :: onEvent(Player_Spawned& evt)
{
	// activate the godmode fx
	m_godModeON = true;
	m_plship = evt.getValue();
}

void MiscFXRenderer :: onEvent(Player_Respawned& evt)
{
	// activate the godmode fx
	m_godModeON = true;
	m_plship = evt.getValue();
}
	
void MiscFXRenderer :: render(Graphics& g) const
{
	if(m_godModeON)
	{
		// depending on time left, alter the length of the rays
		// depending on time left, alter the width of the rays
		// depending on accumulated timer, alter the rotation applied
		// depending on accumulated timer, alter the axis of rotation

		glBlendFunc(GL_SRC_COLOR,GL_ONE);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);

		ShaderManager::instance()->begin("blitShader");
		m_rayTex->bind();
		ShaderManager::instance()->setUniform1i("tex",0);
		

		Vector2 texll(0.0f,0.0f),texext(1.0f,1.0f);
		Vector3 rayUpVector(m_camera->getEye() - m_plship->getCoordinateFrame().getOrigin().getVector());
		rayUpVector.normalize();

		glPushMatrix();
		glTranslatef( m_plship->getCoordinateFrame().getOrigin().getVector().getX(),
					m_plship->getCoordinateFrame().getOrigin().getVector().getY(),
					m_plship->getCoordinateFrame().getOrigin().getVector().getZ());
		glRotatef(m_accumTime*40.0f,1,1,1);

		const float rightMag = RAY_MAX_WIDTH * min(m_plship->getInvulnerableTimeLeft(),0.3f);
		const float upMag = RAY_MAX_LENGTH * min(m_plship->getInvulnerableTimeLeft(),0.5f);

		for(int i=0;i<RAY_NUM;++i)
		{
			Vector3 ll,right,up;

			up = m_rays[i]*upMag;
			right = Vector3::cross(m_rays[i],rayUpVector);
			if(right.dot(right) > 0.0001f)
				right.normalize();
			right *= rightMag;
			ll =  - right*0.5f;	

			RenderEngine::drawTexturedQuad(ll,right,up,texll,texext);
		}

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		glPopMatrix();
	}
}

void MiscFXRenderer :: update(const float dt)
{
	m_accumTime += dt;
	if(m_godModeON)
	{
		if(m_plship && m_plship->isInvulnerable())
			;
		else
			m_godModeON = false;
	}
}