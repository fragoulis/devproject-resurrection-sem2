#include "GameLogic.h"
#include "Terrain.h"
#include "Playership.h"
#include "Enemyship.h"
#include "Spawnpoint.h"
#include "Crater.h"
#include "Ebomb.h"

GameLogic :: GameLogic() : m_terrain(NULL), m_playership(NULL)
{
}

GameLogic :: ~GameLogic()
{
}

void GameLogic :: update(float dt)
{
	for (SpawnpointList::iterator i = m_spawnpoints.begin(); i != m_spawnpoints.end(); ++i)
	{
		(*i)->update(dt);
	}
}


void GameLogic :: loadLevel(const std::string& id)
{
	// should have gotten unload before this
	assert(m_terrain == NULL);

	EventManager::instance().fireEvent(Level_Load(id));

	// TODO: load level data, get terrain id from that
	m_terrain = new Terrain(id);
	EventManager::instance().fireEvent(Terrain_Changed(m_terrain));

	m_playership = new Playership();
	EventManager::instance().fireEvent(Player_Spawned(m_playership));
}

template< typename T >
void deleteObject(T*& t)
{
	if (t != NULL) {
		delete t;
		t = NULL;
	}
}

template< typename T >
void deleteList(std::list<T*>& list)
{
	for (std::list<T*>::iterator i = list.begin(); i != list.end(); ++i)
		delete *i;
	list.clear();
}

void GameLogic :: unloadLevel()
{
	EventManager::instance().fireEvent(Level_Unload());

	deleteObject(m_terrain);
	deleteObject(m_playership);
	deleteList(m_enemyships);
	deleteList(m_spawnpoints);
	deleteList(m_craters);
	deleteList(m_ebombs);
}

void GameLogic :: spawnEnemies(int count, int type)
{
	for (int i = 0; i < count; i++)
	{
		Enemyship* es = new Enemyship(type);
		m_enemyships.push_back(es);
		EventManager::instance().fireEvent(Enemy_Spawned(es));
	}
}

