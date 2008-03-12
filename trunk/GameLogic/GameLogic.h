//*****************************************************************************
//**
//**   File:               GameLogic.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include "../physics/PhysicsEvents.h"
#include <string>
#include <list>
class ParserSection;
class Terrain;
class Playership;
class Enemyship;
class Spawnpoint;
class Crater;
class Ebomb;
class Laser;


/**
 * Keeps track of all game objects (owns them).
 * Handles interactions between multiple game objects
 * Has methods to create/alter/destroy game objects:
 *   spawnEnemies
 *   setPlayerDirection
 *   fireLaser
 *
 * Also has methods to load/unload a level.
 */
class GameLogic :
	public Singleton< GameLogic >,
	public EventListener< Collision_Player_Enemy >
{
public:

	//! Called by Application startup.
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void loadLevel(const std::string& id);
	void unloadLevel();
	Enemyship* spawnEnemy(int type);
	void setPlayerDirection(const Vector3& v);
	void setPlayerThrusterPower(float f);
	void firePositiveLaser(const Point3& targetPosition);
	void fireNegativeLaser(const Point3& targetPosition);
	float getGamePlaneHeight() const { return m_gamePlaneHeight; }
	void update(float dt);

	void onEvent(Collision_Player_Enemy&);

private:

	float m_gamePlaneHeight; // Y coord of all gameplay things like laser, player, enemy

	typedef std::list<Enemyship*> EnemyshipList;
	typedef std::list<Spawnpoint*> SpawnpointList;
	typedef std::list<Crater*> CraterList;
	typedef std::list<Ebomb*> EbombList;
	typedef std::list<Laser*> LaserList;

	// Gameplay objects
	Terrain* m_terrain;
	Playership* m_playership;
	Playership* m_playershipPrototype;
	EnemyshipList m_enemyships;
	SpawnpointList m_spawnpoints;
	CraterList m_craters;
	EbombList m_ebombs;
	LaserList m_lasers;

	// Player laser firing data
	bool m_playerLaserSwapped;
	float m_playerLaserCooldownTime;
	float m_playerLaserCooldownLeft;
	float m_laserStartOffset;
	int m_laserTypePositive;
	int m_laserTypeNegative;

	// helper functions
	void _fireLaser(const Point3& targetPosition, int type);
	template< typename T, typename EventType >
	void _cleanUpList( std::list<T*>& list ); // deletes objects with isToBeDeleted() == true
	void _deleteLevelData();

	// Singleton stuff
	friend Singleton< GameLogic >;
	GameLogic();
	~GameLogic();
};
