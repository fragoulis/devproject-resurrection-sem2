#include "GameLogic.h"
#include "GameEvents.h"
#include "Objects/Terrain.h"
#include "Objects/Playership.h"
#include "Objects/Enemyship.h"
#include "Objects/Spawnpoint.h"
#include "Objects/Crater.h"
#include "Objects/Ebomb.h"
#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfxutils/Misc/utils.h"
#include "../math/Point2.h"
#include "../math/Point3.h"
#include "EnemyTypes.h"
#include <vector>


// Two helpers for deleting objects and lists
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


GameLogic :: GameLogic() : m_terrain(0), m_playership(0), m_playershipPrototype(0)
{
}

GameLogic :: ~GameLogic()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void GameLogic :: onApplicationLoad(const ParserSection& ps)
{
	m_playershipPrototype = new Playership();
	m_playershipPrototype->loadSettings(*ps.getSection("Playership"));
}

void GameLogic :: onApplicationUnload()
{
	_deleteLevelData();
	deleteObject(m_playershipPrototype);
}


/**
 * Responds to collision between player and enemy
 * Reduces player energy by enemy.collisionPower
 * if not enough energy, fires event Player_Destroyed
 * otherwise fires Player_Drained
 */
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

/**
 * Loads a new level
 * Fires Level_Load event.
 * Spawns terrain, playership, craters, spawnpoints
 */
void GameLogic :: loadLevel(const std::string& id)
{
	// should have gotten unload before this
	assert(m_terrain == 0);
	assert(m_playership == 0);
	assert(m_enemyships.empty());

	// load level file
	ConfParser cpLevel(std::string("config/levels/") + id + ".txt");
	const ParserSection* psMap = cpLevel.getSection("map");
	const ParserSection* psFiles = cpLevel.getSection("files");

	// fire event Level_Load
	EventManager::instance().fireEvent(Level_Load(cpLevel.rootSection()));

	// load gameplay file
	std::string filename = std::string("config/levels/") + psFiles->getVal("Gameplay");
	ConfParser cpGameplay(std::string("config/levels/") + psFiles->getVal("Gameplay"));
	const ParserSection* psMainMap = cpGameplay.getSection("main");

	// TODO: get terrain ID from level file
	m_terrain = new Terrain(id);
	EventManager::instance().fireEvent(Terrain_Changed(m_terrain));

	// Spawn player
	m_playership = new Playership(*m_playershipPrototype);
	Point3 pos = FromString<Point3>(psMap->getVal("PlayerStart"));
	m_playership->setPosition(pos);
	EventManager::instance().fireEvent(Player_Spawned(m_playership));

	// Read crater data from gameplay file and spawn craters
	typedef std::vector<const ParserSection *> PSVector;
	const ParserSection* psCraters2 = cpGameplay.getSection("Craters");
	PSVector psCraters = cpGameplay.getSection("Craters")->getChildren();
	for (PSVector::iterator it = psCraters.begin(); it != psCraters.end(); it++)
	{
		Crater* crater = new Crater();
		crater->loadSettings(**it);
		m_craters.push_back(crater);
		EventManager::instance().fireEvent(Crater_Spawned(crater));
	}

	// Read spawnpoint data from gameplay file and spawn spawnpoints
	PSVector psSpawnpoints = cpGameplay.getSection("Spawnpoints")->getChildren();
	for (PSVector::iterator it = psSpawnpoints.begin(); it != psSpawnpoints.end(); it++)
	{
		Spawnpoint* spawnPoint = new Spawnpoint();
		spawnPoint->loadSettings(**it);
		m_spawnpoints.push_back(spawnPoint);
		EventManager::instance().fireEvent(Spawnpoint_Spawned(spawnPoint));
	}
}

void GameLogic :: _deleteLevelData()
{
	deleteObject(m_terrain);
	deleteObject(m_playership);
	deleteList(m_enemyships);
	deleteList(m_spawnpoints);
	deleteList(m_craters);
	deleteList(m_ebombs);
}

void GameLogic :: unloadLevel()
{
	EventManager::instance().fireEvent(Level_Unload());
	_deleteLevelData();
}

void GameLogic :: spawnEnemies( int count, int type )
{
	EnemyTypes& enemyTypes = EnemyTypes::instance();
	for (int i = 0; i < count; i++)
	{
		Enemyship* es = enemyTypes.createEnemyship(type);
		m_enemyships.push_back(es);
		EventManager::instance().fireEvent(Enemy_Spawned(es));
	}
}


void GameLogic :: setPlayerDirection( const Vector3& v )
{
	assert(m_playership != 0);
	m_playership->setThrusterDirection(v);
}

void GameLogic :: fireLaser(const Point3& target, LaserType type)
{
	const Point3& playerPosition = m_playership->getPosition();
	Vector3 direction = target - playerPosition;
	direction.normalize();
}