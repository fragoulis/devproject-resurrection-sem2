#include "ClampRenderer.h"
#include "RenderEngine.h"

static const float ATTACHMENT_WIDTH = 10.0f;

using namespace std;

ClampRenderer :: ClampRenderer()
:m_accumTimer(0.0f),
m_plship(NULL)
{
	EventManager::instance().registerEventListener< Enemy_Attached_To_Player >(this);
	EventManager::instance().registerEventListener< Enemy_Destroyed >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);

	// get appropriate texture here
}

ClampRenderer :: ~ClampRenderer()
{
	// delete anything applicable
}

void ClampRenderer :: onEvent(Enemy_Attached_To_Player& evt)
{
	// add the pair

	// sanity check
	if(m_plship)
		assert(evt.getValue2() == m_plship);
	else
		m_plship = evt.getValue2();
	m_enships.push_back(evt.getValue1());	
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

void ClampRenderer :: render(Graphics& g) const
{
	// render pairs

	// set texture - shader - parameters

	for(vector<const Enemyship *>::const_iterator it = m_enships.begin();
		it != m_enships.end();
		++it)
	{
		// get data & render
	}
}

void ClampRenderer :: update(const float dt)
{
	// update timer for timed effect on clamping
	m_accumTimer += dt;
}