//*****************************************************************************
//**
//**   File:               GameLogic.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "GameLogic.h"
#include "GameEvents.h"
#include "Objects/Terrain.h"
#include "Objects/Playership.h"
#include "Objects/Spawnpoint.h"
#include "Objects/Crater.h"
#include "Objects/Ebomb.h"
#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfxutils/Misc/utils.h"
#include "../math/Point2.h"
#include "../math/Point3.h"
#include "Enemies/Enemyship.h"
#include "Enemies/EnemyFactory.h"
#include "Lasers/Laser.h"
#include "Lasers/LaserFactory.h"
#include "WorldObjectTypeManager.h"
#include "../utility/deleters.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include <vector>
#include <iostream>
using namespace std;



GameLogic :: GameLogic() : 
	m_terrain(0),
	m_playership(0),
	m_playershipPrototype(0)
{
}

GameLogic :: ~GameLogic()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void GameLogic :: onApplicationLoad(const ParserSection& ps)
{
	EventManager::instance().registerEventListener< Collision_Player_Enemy >(this);

	const ParserSection* psPlayer = ps.getSection("Playership");
	m_playershipPrototype = new Playership();
	m_playershipPrototype->loadSettings(*psPlayer);
	m_playershipPrototype->setType(WorldObjectTypeManager::instance().getTypeFromName("PlayerShip"));

	m_playerLaserCooldownTime = FromString<float>(psPlayer->getVal("LaserCooldown"));
	m_laserStartOffset = FromString<float>(psPlayer->getVal("LaserStartOffset"));

	m_laserTypePositive = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerPositive");
	m_laserTypeNegative = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerNegative");
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
		enemy->setToBeDeleted();
	}
}

void GameLogic :: update(float dt)
{
	// Laser cooldown
	m_playerLaserCooldownLeft -= dt;

	// Send update to objects that need it
	for (SpawnpointList::iterator i = m_spawnpoints.begin(); i != m_spawnpoints.end(); ++i)
	{
		(*i)->update(dt, m_playership->getPosition());
	}
	for (LaserList::iterator it = m_lasers.begin(); it != m_lasers.end(); ++it)
	{
		(*it)->update(dt);
	}

	// delete objects that died this round
	_cleanUpList<Enemyship, Enemy_Despawned>(m_enemyships);
	_cleanUpList<Ebomb, Ebomb_Despawned>(m_ebombs);
	_cleanUpList<Laser, Laser_Despawned>(m_lasers);

	// Output debug info
	//cout << m_playership->getPosition() << endl;
}

Enemyship* GameLogic :: spawnEnemy( int type )
{
	Enemyship* es = EnemyFactory::instance().createEnemyship(type);
	EventManager::instance().fireEvent(Enemy_Spawned(es));
	m_enemyships.push_back(es);
	return es;
}


void GameLogic :: setPlayerDirection( const Vector3& v )
{
	assert(m_playership != 0);
	m_playership->setThrusterDirection(v);
	m_playership->setThrusterPower(m_playership->getMaxThrusterPower());
}

void GameLogic :: setPlayerThrusterPower(const float f)
{
	m_playership->setThrusterPower(f);
}

void GameLogic :: firePositiveLaser(const Point3& targetPosition) {
	_fireLaser(targetPosition, m_laserTypePositive);
}

void GameLogic :: fireNegativeLaser(const Point3& targetPosition) {
	_fireLaser(targetPosition, m_laserTypeNegative);
}

void GameLogic :: _fireLaser(const Point3& target, int type)
{
	if (m_playerLaserCooldownLeft <= 0.0f) {
		const Point3& playerPosition = m_playership->getPosition();
		Vector3 direction = target - playerPosition;
		direction.normalize();
		Point3 startingPosition = playerPosition + direction * m_laserStartOffset;
		if (m_playerLaserSwapped) {
			if (type == m_laserTypePositive) type = m_laserTypeNegative;
			else type = m_laserTypePositive;
		}
		Laser* laser = LaserFactory::instance().createLaser(type);
		laser->start(startingPosition, direction);
		m_lasers.push_back(laser);
		m_playerLaserCooldownLeft = m_playerLaserCooldownTime;
		EventManager::instance().fireEvent(Laser_Spawned(laser));
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
	const ParserSection* psMap = cpLevel.getSection("Map");
	const ParserSection* psGameplay = cpLevel.getSection("Gameplay");

	// fire event Level_Load
	EventManager::instance().fireEvent(Level_Load(&cpLevel.rootSection(), id));

	// load gameplay file
	ConfParser cpGameplay(std::string("config/levels/") + psGameplay->getVal("file"));
	const ParserSection* psMainMap = cpGameplay.getSection("main");

	// create terrain, fire event Terrain_Changed
	m_terrain = new Terrain();
	EventManager::instance().fireEvent(Terrain_Changed(m_terrain));

	// Spawn player, fire event Player_Spawned
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


	// Reset variables
	m_playerLaserSwapped = false;
	m_playerLaserCooldownLeft = 0.0f;
}


void GameLogic :: unloadLevel()
{
	EventManager::instance().fireEvent(Level_Unload());
	_deleteLevelData();
	m_playerLaserSwapped = false;
}


void GameLogic :: _deleteLevelData()
{
	deleteObject(m_terrain);
	deleteObject(m_playership);
	deleteList(m_enemyships);
	deleteList(m_spawnpoints);
	deleteList(m_craters);
	deleteList(m_ebombs);
	deleteList(m_lasers);
}





// This can be rewritten to work with vectors:
// use integer index as loop iterator
// swap back() with current iteration and redo current iteration
template< typename T, typename EventType >
void GameLogic :: _cleanUpList( std::list<T*>& list )
{
	typedef std::list<T*> List;
	for (List::iterator it = list.begin(); it != list.end(); )
	{
		T* t = *it;
		if (t->isToBeDeleted()) {
			CKLOG(std::string("Firing despawn event for ") + ToString<T*>(t), 3);
			it = list.erase(it);
			EventManager::instance().fireEvent(EventType(t));
			delete t;
		}
		else {
			++it;
		}
	}
}
