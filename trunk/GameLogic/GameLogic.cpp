//*****************************************************************************
//**
//**   File:               GameLogic.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
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
#include "Buffs/BuffFactory.h"
#include "../utility/deleters.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include <vector>
#include <iostream>
using namespace std;



GameLogic :: GameLogic() : 
	m_terrain(0),
	m_playership(0),
	m_playershipPrototype(0),
	m_currentEbomb(EBOMB_TYPE_UNKNOWN),
	m_lasersSwapped(false)
{
}

GameLogic :: ~GameLogic()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void GameLogic :: onApplicationLoad(const ParserSection& ps)
{
	// Register for event listening
	EventManager::instance().registerEventListener< Collision_Player_Enemy >(this);
	EventManager::instance().registerEventListener< Collision_Enemy_Laser >(this);
	EventManager::instance().registerEventListener< Collision_Ebomb_Crater >(this);
	EventManager::instance().registerEventListener< Collision_Ebomb_Terrain >(this);
	EventManager::instance().registerEventListener< Player_EnergyGained >(this);
	EventManager::instance().registerEventListener< Player_EnergyDrained >(this);
	EventManager::instance().registerEventListener< Player_EnergyDispersed >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);

	// Load data
	const ParserSection* psGame = ps.getSection("main");

	// Load playership data
	const ParserSection* psPlayer = ps.getSection("Playership");
	m_playershipPrototype = new Playership();
	m_playershipPrototype->loadSettings(*psPlayer);

	// load laser data
	const ParserSection* psLaser = ps.getSection("Laser");

	m_playerLaserCooldownTime = FromString<float>(psLaser->getVal("Cooldown"));
	m_laserStartOffset = FromString<float>(psLaser->getVal("StartOffset"));
	m_laserPowerFactor = FromString<float>(psLaser->getVal("PowerFactor"));
	m_laserTypePositive = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerPositive");
	m_laserTypeNegative = WorldObjectTypeManager::instance().getTypeFromName("LaserPlayerNegative");
	m_laserSwapDebuffType = BuffFactory::instance().getTypeFromName("LaserSwap");

	for (int type = 0; type < ENERGY_TYPE_COUNT; type++)
	{
		std::string name = std::string("LaserPower") + CStringFromEnergyType(EnergyType(type));
		m_laserPowerType[type] = BuffFactory::instance().getTypeFromName(name);
	}


	// load e-bomb data
	const ParserSection* psEbomb = ps.getSection("Ebomb");
	m_ebombPrototype = new Ebomb();
	m_ebombPrototype->loadSettings(*psEbomb);
	m_normalBombEnergy = m_playershipPrototype->getEnergyCapacity();
	m_combinedBombEnergy = int(m_playershipPrototype->getEnergyCapacity() / 2);
	m_ebombInitialDownwardVelocity = FromString<float>(psEbomb->getVal("InitialDownwardVelocity"));
}

void GameLogic :: onApplicationUnload()
{
	_deleteLevelData();
	deleteObject(m_playershipPrototype);
	deleteObject(m_ebombPrototype);
}


/**
 * Responds to collision between player and enemy
 * Reduces player energy by enemy.collisionPower
 * if not enough energy, fires event Player_Destroyed
 * otherwise fires Player_Drained
 * This code is now located in EnemyShip and its subclasses
 */
void GameLogic :: onEvent(Collision_Player_Enemy& coldata)
{
	Playership* player = coldata.getObject1();
	Enemyship* enemy = coldata.getObject2();
	enemy->collideWithPlayer(player);
}

void GameLogic :: onEvent(Player_Destroyed& evt)
{
	Playership* player = evt.getValue1();
	m_currentLives--;
	if (m_currentLives == 0) {
		EventManager::instance().fireEvent(Game_Over());
	}
	else {
		player->respawn();
		EventManager::instance().fireEvent(Player_Respawned(player));
	}
}

/**
 * Responds to collision between laser and enemy
 * Reduces enemy hitpoints by 1 and kills laser.
 * If enemy is destroyed, action depends on type of laser.
 * If positive, adds energy to player.
 * If negative, reduces energy from player, but will not destroy player.
 */
void GameLogic :: onEvent( Collision_Enemy_Laser& evt )
{
	Enemyship* enemy = evt.getObject1();
	Laser* laser = evt.getObject2();
	int laserType = laser->getType();

	// reduce enemy hitpoints by 1
	EnergyType type = enemy->getEnergyType();
	int power = m_playership->getBuffStacks(m_laserPowerType[type]);
	enemy->reduceHitPoints(1.0f + power * m_laserPowerFactor);

	// check if enemy can sustain the damage taken
	if (enemy->getHitPoints() <= 0) {
		int oldPlayerEnergy = m_playership->getEnergy(type);
		int enemyEnergy = enemy->getEnergyPoints();

		// remove enemy from game
		enemy->setToBeDeleted();
		EventManager::instance().fireEvent(Enemy_Destroyed(enemy));

		// Check laser type
		// We don't create e-bombs here, that will be done in response to Player_EnergyGained
		if (laserType == m_laserTypePositive) {
			int addedEnergy = min(enemyEnergy, m_playership->getEnergyCapacity() - m_playership->getTotalEnergy());
			if (addedEnergy > 0) {
				m_playership->setEnergy(type, oldPlayerEnergy + addedEnergy);
				EventManager::instance().fireEvent(Player_EnergyGained(m_playership, type, addedEnergy));
			}
		}
		else {
			int newPlayerEnergy = max(oldPlayerEnergy - enemyEnergy, 0);
			if (newPlayerEnergy < oldPlayerEnergy) {
				m_playership->setEnergy(type, newPlayerEnergy);
				EventManager::instance().fireEvent(Player_EnergyDispersed(m_playership, type, oldPlayerEnergy - newPlayerEnergy));
			}
		}
	}
	else {
		EventManager::instance().fireEvent(Enemy_Hit(enemy, laserType));
	}

	// kill the laser
	laser->die();
}












/*********************************************************************

Piece of code below takes care of creating and uncreating
e-bombs in player's cargo bay

**********************************************************************/


void GameLogic :: onEvent( Player_EnergyGained& evt )
{
	cerr << "Gained " << evt.getValue3() << StringFromEnergyType(evt.getValue2()) << " energy" << endl;

	// if we aready have an e-bomb, we can't create another
	if (m_currentEbomb != EBOMB_TYPE_UNKNOWN) return;

	EbombType ebombType = _seeIfPlayerCanCreateEbombAndReturnTypeOfBomb();
	if (ebombType != EBOMB_TYPE_UNKNOWN) {
		m_currentEbomb = ebombType;
		EventManager::instance().fireEvent(Ebomb_Created(ebombType));
		cerr << "Ebomb of type " << StringFromEbombType(ebombType) << " created" << endl;
	}
}
void GameLogic :: onEvent( Player_EnergyDrained& evt )
{
	cerr << "Drained " << evt.getValue3() << StringFromEnergyType(evt.getValue2()) << " energy" << endl;
	_checkEbombUncreation();
}
void GameLogic :: onEvent( Player_EnergyDispersed& evt )
{
	cerr << "Dispersed " << evt.getValue3() << StringFromEnergyType(evt.getValue2()) << " energy" << endl;
	_checkEbombUncreation();
}

EbombType GameLogic :: _seeIfPlayerCanCreateEbombAndReturnTypeOfBomb()
{
	if (_checkNormalEbombCreation(ENERGY_TYPE_RED)) return EBOMB_TYPE_RED;
	if (_checkNormalEbombCreation(ENERGY_TYPE_YELLOW)) return EBOMB_TYPE_YELLOW;
	if (_checkNormalEbombCreation(ENERGY_TYPE_BLUE)) return EBOMB_TYPE_BLUE;
	if (_checkCombinedEbombCreation(ENERGY_TYPE_RED, ENERGY_TYPE_YELLOW)) return EBOMB_TYPE_ORANGE;
	if (_checkCombinedEbombCreation(ENERGY_TYPE_RED, ENERGY_TYPE_BLUE)) return EBOMB_TYPE_PURPLE;
	if (_checkCombinedEbombCreation(ENERGY_TYPE_YELLOW, ENERGY_TYPE_BLUE)) return EBOMB_TYPE_GREEN;
	return EBOMB_TYPE_UNKNOWN;
}

bool GameLogic :: _checkNormalEbombCreation(EnergyType energyType)
{
	int energy = m_playership->getEnergy(energyType);
	return energy == m_normalBombEnergy;
}

bool GameLogic :: _checkCombinedEbombCreation(EnergyType energyType1, EnergyType energyType2)
{
	int energy1 = m_playership->getEnergy(energyType1);
	int energy2 = m_playership->getEnergy(energyType2);
	return energy1 == m_combinedBombEnergy && energy2 == m_combinedBombEnergy;
}

void GameLogic :: _checkEbombUncreation()
{
	// if we don't have an e-bomb, we can't uncreate it
	if (m_currentEbomb == EBOMB_TYPE_UNKNOWN) return;

	if (_seeIfPlayerCanCreateEbombAndReturnTypeOfBomb() == EBOMB_TYPE_UNKNOWN) {
		EbombType ebombType = m_currentEbomb;
		m_currentEbomb = EBOMB_TYPE_UNKNOWN;
		EventManager::instance().fireEvent(Ebomb_Uncreated(ebombType));
		cerr << "Ebomb of type " << StringFromEbombType(ebombType) << "uncreated" << endl;
	}
}














/**
 * Attempts to drop an e-bomb at specified location
 * If no e-bomb is available, returns without trying anything.
 * E-bomb will drop straight down due to gravity
 * Physics will check collision with craters and fire event
 */
void GameLogic :: dropEbomb(const Point3& targetLocation)
{
	// check if we have a bomb available
	if (m_currentEbomb == EBOMB_TYPE_UNKNOWN) return;

	Ebomb* ebomb = new Ebomb(*m_ebombPrototype);
	ebomb->setEbombType(m_currentEbomb);
	ebomb->setPosition(targetLocation);
	ebomb->setY(m_gamePlaneHeight);
	ebomb->setVelocity(Vector3(0.0f, -m_ebombInitialDownwardVelocity, 0.0f));
	m_ebombs.push_back(ebomb);

	cerr << "Ebomb of type " << StringFromEbombType(m_currentEbomb) << " dropped" << endl;

	m_currentEbomb = EBOMB_TYPE_UNKNOWN;
	m_playership->resetAllEnergy();

	EventManager::instance().fireEvent(Ebomb_Spawned(ebomb));
}


void GameLogic :: onEvent( Collision_Ebomb_Crater& evt )
{
	Ebomb* ebomb = evt.getObject1();
	Crater* crater = evt.getObject2();
	ebomb->setToBeDeleted();
	if (ebomb->getEbombType() == crater->getEbombType())
	{
		EventManager::instance().fireEvent(Life_Restored(crater));
		crater->setToBeDeleted();
		_addLaserPowerBuffs(ebomb->getEbombType());

		// some craters may still be alive (aside from this one),
		// but are set to be deleted
		int unfixedCraters = 0;
		for (CraterList::iterator it = m_craters.begin(); it != m_craters.end(); ++it)
		{
			Crater* crater = *it;
			if (!crater->isToBeDeleted()) unfixedCraters++;
		}
		if (unfixedCraters == 0) EventManager::instance().fireEvent(Level_Complete());
	}
	else
	{
		EventManager::instance().fireEvent(Ebomb_Missed(ebomb));
	}
}

void GameLogic :: onEvent( Collision_Ebomb_Terrain& evt )
{
	Ebomb* ebomb = evt.getObject1();
	ebomb->setToBeDeleted();
	EventManager::instance().fireEvent(Ebomb_Missed(ebomb));
}

void GameLogic :: _addLaserPowerBuffs(EbombType ebombType)
{
	int type1, type2;
	switch (ebombType)
	{
		case EBOMB_TYPE_RED :
		{
			type1 = m_laserPowerType[ENERGY_TYPE_RED];
			type2 = m_laserPowerType[ENERGY_TYPE_RED];
			break;
		}
		case EBOMB_TYPE_YELLOW :
		{
			type1 = m_laserPowerType[ENERGY_TYPE_YELLOW];
			type2 = m_laserPowerType[ENERGY_TYPE_YELLOW];
			break;
		}
		case EBOMB_TYPE_BLUE :
		{
			type1 = m_laserPowerType[ENERGY_TYPE_BLUE];
			type2 = m_laserPowerType[ENERGY_TYPE_BLUE];
			break;
		}
		case EBOMB_TYPE_ORANGE :
		{
			type1 = m_laserPowerType[ENERGY_TYPE_RED];
			type2 = m_laserPowerType[ENERGY_TYPE_YELLOW];
			break;
		}
		case EBOMB_TYPE_GREEN :
		{
			type1 = m_laserPowerType[ENERGY_TYPE_YELLOW];
			type2 = m_laserPowerType[ENERGY_TYPE_BLUE];
			break;
		}
		case EBOMB_TYPE_PURPLE :
		{
			type1 = m_laserPowerType[ENERGY_TYPE_RED];
			type2 = m_laserPowerType[ENERGY_TYPE_BLUE];
			break;
		}
	}
	m_playership->addBuff(type1);
	m_playership->addBuff(type2);
}














/**
 * Send update on to objects that need it.
 * Deletes objects that want to be deleted (isToBeDeleted() returns true)
 */
void GameLogic :: update(float dt)
{
	// Laser cooldown
	m_playerLaserCooldownLeft -= dt;

	// Send update to objects that need it
	m_playership->update(dt);

	int width = int(m_terrain->getTerrainWidth());
	int height = int(m_terrain->getTerrainHeight());

	Point2 minpoint(-width * 13.5f, -height * 12.0f);
	Point2 maxpoint(width * 13.5f, height * 15.0f);
	m_playership->confine(minpoint, maxpoint);
	for (SpawnpointList::iterator i = m_spawnpoints.begin(); i != m_spawnpoints.end(); ++i)
	{
		(*i)->update(dt, m_playership->getPosition());
	}
	for (LaserList::iterator it = m_lasers.begin(); it != m_lasers.end(); ++it)
	{
		(*it)->update(dt);
	}
	for (EnemyshipList::iterator i = m_enemyships.begin(); i != m_enemyships.end(); ++i)
	{
		(*i)->update(dt);
	}

	// delete objects that died this round
	_cleanUpList<Enemyship, Enemy_Despawned>(m_enemyships);
	_cleanUpList<Ebomb, Ebomb_Despawned>(m_ebombs);
	_cleanUpList<Laser, Laser_Despawned>(m_lasers);
	_cleanUpList<Crater, Crater_Despawned>(m_craters);

	// Output debug info
	//cout << m_playership->getPosition() << endl;
}







/**
 * Helper function for spawn points
 */
Enemyship* GameLogic :: spawnEnemy( int type )
{
	Enemyship* es = EnemyFactory::instance().createEnemyship(type);
	m_enemyships.push_back(es);
	Point3 pos = es->getPosition();
	pos.setY(m_gamePlaneHeight);
	es->setPosition(pos);
	return es;
}

void GameLogic :: swapLasers()
{
	m_lasersSwapped = true;
	EventManager::instance().fireEvent(Player_Laser_Swapped());
}

void GameLogic :: unSwapLasers()
{
	m_lasersSwapped = false;
}


/**
 * Sets player direction, called by controller
 */
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
		startingPosition.setY(m_gamePlaneHeight);

		// same thing :-/
		if (m_lasersSwapped) {
		//if (m_playership->hasBuff(m_laserSwapDebuffType)) {
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

	// Reset variables
	m_playerLaserCooldownLeft = 0.0f;
	m_currentEbomb = EBOMB_TYPE_UNKNOWN;


	// load level file
	ConfParser cpLevel(std::string("config/levels/") + id + ".txt");
	const ParserSection* psMap = cpLevel.getSection("Map");
	const ParserSection* psGameplay = cpLevel.getSection("Gameplay");

	// fire event Level_Load
	EventManager::instance().fireEvent(Level_Load(&cpLevel.rootSection(), id));

	// load gameplay file
	ConfParser cpGameplay(std::string("config/levels/") + psGameplay->getVal("file"));
	const ParserSection* psGameplayMain = cpGameplay.getSection("Main");
	m_maxLives = FromString<int>(psGameplayMain->getVal("Lives"));
	m_currentLives = m_maxLives;
	m_gamePlaneHeight = FromString<float>(psGameplayMain->getVal("GamePlaneHeight"));


	// create terrain, fire event Terrain_Changed
	m_terrain = new Terrain();
	EventManager::instance().fireEvent(Terrain_Changed(m_terrain));

	// Spawn player, fire event Player_Spawned
	m_playership = new Playership(*m_playershipPrototype);
	Point2 pos = FromString<Point2>(psMap->getVal("PlayerStart"));
	m_playership->setX(pos.getX());
	m_playership->setY(m_gamePlaneHeight);
	m_playership->setZ(pos.getY());
	m_playership->respawn();
	EventManager::instance().fireEvent(Player_Spawned(m_playership));

	// Read crater data from gameplay file and spawn craters
	typedef std::vector<const ParserSection *> PSVector;
	const ParserSection* psCraters2 = cpGameplay.getSection("Craters");
	PSVector psCraters = cpGameplay.getSection("Craters")->getChildren();
	for (PSVector::iterator it = psCraters.begin(); it != psCraters.end(); it++)
	{
		Crater* crater = new Crater();
		crater->loadSettings(**it);
		crater->setY(m_terrain->getHeight(crater->getX(), crater->getZ()));
		m_craters.push_back(crater);
		EventManager::instance().fireEvent(Crater_Spawned(crater));
	}

	// Read spawnpoint data from gameplay file and spawn spawnpoints
	PSVector psSpawnpoints = cpGameplay.getSection("Spawnpoints")->getChildren();
	for (PSVector::iterator it = psSpawnpoints.begin(); it != psSpawnpoints.end(); it++)
	{
		Spawnpoint* spawnPoint = new Spawnpoint();
		spawnPoint->loadSettings(**it);
		spawnPoint->setY(m_gamePlaneHeight);
		m_spawnpoints.push_back(spawnPoint);
		EventManager::instance().fireEvent(Spawnpoint_Spawned(spawnPoint));
	}
}


void GameLogic :: unloadLevel()
{
	EventManager::instance().fireEvent(Level_Unload());
	_deleteLevelData();
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


float GameLogic :: getTerrainHeight(float x, float z)
{
	return m_terrain->getHeight(x, z);
}