#include "ClampRenderer.h"
#include "RenderEngine.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Objects/Playership.h"
#include <gl/glee.h>
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include "../gfx/Shaders/ShaderManager.h"

static const float ATTACHMENT_WIDTH = 40.0f;

using namespace std;

ClampRenderer :: ClampRenderer()
:m_accumTimer(0.0f),
m_plship(NULL)
{
	EventManager::instance().registerEventListener< Interceptor_Clamped >(this);
	EventManager::instance().registerEventListener< Enemy_Destroyed >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Respawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_Despawned >(this);

	// get appropriate texture here
}

ClampRenderer :: ~ClampRenderer()
{
	// delete anything applicable
}

void ClampRenderer :: onEvent(Interceptor_Clamped& evt)
{
	// add 
	m_enships.push_back(evt.getValue());	
}

void ClampRenderer :: onEvent(Enemy_Destroyed& evt)
{
	// delete the pair
	for(vector<const Enemyship *>::iterator it = m_enships.begin();
		it != m_enships.end();
		++it)
	{
		if((*it) == evt.getValue())
		{
			m_enships.erase(it);
			break;
		}
	}
}

void ClampRenderer :: onEvent(Enemy_Despawned& evt)
{
	// delete the pair
	for(vector<const Enemyship *>::iterator it = m_enships.begin();
		it != m_enships.end();
		++it)
	{
		if((*it) == evt.getValue())
		{
			m_enships.erase(it);
			break;
		}
	}
}

void ClampRenderer :: onEvent(Player_Spawned& evt)
{
	//set the ship
	m_plship = evt.getValue();
}

void ClampRenderer :: onEvent(Player_Respawned& evt)
{
	//set the ship
	m_plship = evt.getValue();
}

void ClampRenderer :: onEvent(Player_Destroyed& evt)
{
	// remove all the pairs
	m_plship = NULL;
	m_enships.clear();
}

void ClampRenderer :: onEvent(Player_Despawned& evt)
{
	// remove all the pairs
	m_plship = NULL;
	m_enships.clear();
}

void ClampRenderer :: render(Graphics& g) const
{
	// render pairs
	if((!m_plship) || m_enships.empty())
		return;

	// set texture - shader - parameters
	glBlendFunc(GL_SRC_COLOR,GL_ONE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	ShaderManager::instance()->begin("blitShader");
	TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getNoiseTexture1D())->bind();
	ShaderManager::instance()->setUniform1i("tex",0);

	const Vector3& plpos(m_plship->getCoordinateFrame().getOrigin().getVector());
	
	const Vector2 texll(0.0f + m_accumTimer,0.0f);
	const Vector2 texext(1.0f,1.0f);

	for(vector<const Enemyship *>::const_iterator it = m_enships.begin();
		it != m_enships.end();
		++it)
	{
		const Enemyship * enemy(*it);
		// get data & render
		const Vector3& enpos(enemy->getCoordinateFrame().getOrigin().getVector());
		
		Vector3 up(plpos - enpos);
		Vector3 right(Vector3::cross(up,Vector3(0.0f,1.0f,0.0f)));
		right.normalize();
		right*= ATTACHMENT_WIDTH;
		Vector3 ll(enpos - right*0.5f);

		RenderEngine::drawTexturedQuad(ll,right,up,texll,texext);
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ClampRenderer :: update(const float dt)
{
	// update timer for timed effect on clamping
	m_accumTimer += dt;
}