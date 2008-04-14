#include "SpawnPointRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../math/Vector3.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../GameLogic/GameLogic.h"

#include "../gfxutils/Misc/Logger.h"

using namespace std;

SpawnPointRenderer :: SpawnPointRenderer()
{
	EventManager::instance().registerEventListener< Spawnpoint_Spawned >(this);
	EventManager::instance().registerEventListener< Spawnpoint_Despawned >(this);
	EventManager::instance().registerEventListener< Spawnpoint_SessionStarted >(this);
	EventManager::instance().registerEventListener< Spawnpoint_SessionEnded >(this);

	m_spawnPointTexture = RenderEngine::instance().getConstRenderSettings().getSpawnPointTexture();
}

void SpawnPointRenderer :: render(Graphics& g) const
{
	const float interval = RenderEngine::instance().getConstRenderSettings().getSpawnPointInterval();
	const float size = RenderEngine::instance().getConstRenderSettings().getSpawnPointSize();
	const float height = GameLogic::instance().getGamePlaneHeight();

	Vector3 right(size,0.0f,0.0f);
	Vector3 up(0.0f,0.0f,-size);
	Vector2 ll_tex(0.0f,0.0f);
	Vector2 ur_tex(1.0f,1.0f);

	ShaderManager::instance()->begin("SpawnPointShader");
	m_spawnPointTexture->bind();
	ShaderManager::instance()->setUniform1i("texmap0",0);

	/*
		The shader will take as input a range from 0..1
		0 is closed portal
		1 is open portal
		The TIME interval for those, in order to convert, will be given ny RenderEngine settings
		Also the width will be given from there
	*/

	// don't write to depth buffer
	glDepthMask(GL_FALSE);
	//glAlphaFunc(GL_GREATER,0.1f);
	//glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	unsigned i=0;
	for(std::vector<SpawnPointInfo_t>::const_iterator it = m_spawnPoints.begin();
		it != m_spawnPoints.end();
		++it)
	{
		if(it->isActive)
		{
			// pass the elapsed time / interval if not ended, inverse otherwise
			// draw the quad

			float timepcent = it->elapsedTime / interval;
			float extra = timepcent - min(timepcent,1.0f);
			timepcent -= extra;
			if(it->hasEnded)
			{
				timepcent = 1.0f - timepcent;
				timepcent -= extra;
			}
			else
				timepcent += extra;
			ShaderManager::instance()->setUniform1fv("timePercent",&timepcent);

			const Vector3 ll(it->sp->getCoordinateFrame().getOrigin().getVector() - Vector3(size*0.5f,0.0f,size*0.5f));
			RenderEngine::drawTexturedQuad(ll,right,up,ll_tex,ur_tex);
		}
	}
	//glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);
}

void SpawnPointRenderer :: update(const float dt)
{
	// keep a track of time for session starting / ending
	// one for each spawnpoint , probably in a list.
	const float interval = RenderEngine::instance().getConstRenderSettings().getSpawnPointInterval();
	for(std::vector<SpawnPointInfo_t>::iterator it = m_spawnPoints.begin();
		it != m_spawnPoints.end();
		++it)
	{	
		it->elapsedTime += dt;
		if((it->elapsedTime > interval) && it->hasEnded)
			it->isActive = false;
	}
}


void SpawnPointRenderer :: onEvent(Spawnpoint_Spawned& evt)
{
	m_spawnPoints.push_back(SpawnPointInfo_t(evt.getValue()));
}

void SpawnPointRenderer :: onEvent(Spawnpoint_Despawned& evt)
{
	const Spawnpoint * sp = evt.getValue();
	for(std::vector<SpawnPointInfo_t>::iterator it = m_spawnPoints.begin();
		it != m_spawnPoints.end();
		++it)
	{
		if(it->sp == sp)
		{
			m_spawnPoints.erase(it);
			return;
		}
	}
	assert(0);
}


void SpawnPointRenderer :: onEvent(Spawnpoint_SessionStarted& evt)
{
	// set as active & reset it's timer
	const Spawnpoint * sp = evt.getValue();
	for(std::vector<SpawnPointInfo_t>::iterator it = m_spawnPoints.begin();
		it != m_spawnPoints.end();
		++it)
	{
		if(it->sp == sp)
		{
			it->isActive = true;
			it->elapsedTime = 0.0f;
			it->hasEnded = false;
			return;
		}
	}
	assert(0);
}

void SpawnPointRenderer :: onEvent(Spawnpoint_SessionEnded& evt)
{
	// set as closing & reset it's timer
	const Spawnpoint * sp = evt.getValue();
	for(std::vector<SpawnPointInfo_t>::iterator it = m_spawnPoints.begin();
		it != m_spawnPoints.end();
		++it)
	{
		if(it->sp == sp)
		{
			it->elapsedTime = 0.0f;
			it->hasEnded = true;
			return;
		}
	}
	assert(0);
}