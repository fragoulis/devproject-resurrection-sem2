#include "GameLogic.h"
#include "GameEvents.h"
#include "Objects/Terrain.h"
#include "Objects/Playership.h"
#include "Objects/Enemyship.h"
#include "Objects/Spawnpoint.h"
#include "Objects/Crater.h"
#include "Objects/Ebomb.h"
#include "../gfxutils/ConfParser/ConfParser.h"

GameLogic :: GameLogic() : m_terrain(NULL), m_playership(NULL)
{
}

GameLogic :: ~GameLogic()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void GameLogic :: onApplicationLoad(const ParserSection& ps)
{
	// TODO: read ParserSection for info and load data from disk
}

void GameLogic :: onApplicationUnload()
{
	// TODO: delete allocated memory
}



void GameLogic :: onEvent(Collision_Player_Enemy& coldata)
{
	Playership* player = coldata.getObject1();
	Enemyship* enemy = coldata.getObject2();

	EnergyType type = enemy->getEnergyType();
	int energy = player->getEnergy(type);
	int colpower = enemy->getCollisionPower();
	energy -= colpower;

	if (energy <= 0) {
		// TODO: reset player to some position, set it to invulnerable for a few sec
		EventManager::instance().fireEvent(Player_Destroyed(player, type));
	}
	else {
		player->setEnergy(type, energy);
		EventManager::instance().fireEvent(Player_Drained(player, type, colpower));
	}
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
	assert(m_playership == NULL);
	assert(m_enemyships.empty());

	ConfParser cp(std::string("./config/") + id + ".txt");
	const ParserSection& ps = cp.rootSection();

	EventManager::instance().fireEvent(Level_Load(ps));

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

