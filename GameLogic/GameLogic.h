//*****************************************************************************
//**
//**   File:               GameLogic.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


#pragma once
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include "../physics/PhysicsEvents.h"
#include "GameEvents.h"
#include "EbombTypes.h"
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
 *   dropEbomb
 *
 * Also has methods to load/unload a level.
 */
class GameLogic :
	public Singleton< GameLogic >,
	public EventListener< Collision_Player_Enemy >,
	public EventListener< Collision_Enemy_Laser >,
	public EventListener< Collision_Ebomb_Crater >,
	public EventListener< Collision_Ebomb_Terrain >,
	public EventListener< Player_EnergyGained >,
	public EventListener< Player_EnergyDrained >,
	public EventListener< Player_EnergyDispersed >
{
public:

	//! Called by Application startup.
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	//! Called by GameController. To pause game, controller can stop calling this
	void update(float dt);

	float getGamePlaneHeight() const { return m_gamePlaneHeight; }

	//! Retrieves type of e-bomb in player's cargo bay. Returns unknown if no bomb is available.
	EbombType getCurrentEbombType() const { return m_currentEbomb; }
	int getCurrentLives() const { return m_currentLives; }
	const Playership* getPlayership() const { return m_playership; }
	float getTerrainHeight(float x, float z);


	// These are functions controller can call to change the game state
	void loadLevel(const std::string& id);
	void unloadLevel();
	Enemyship* spawnEnemy(int type);
	void setPlayerDirection(const Vector3& v);
	void setPlayerThrusterPower(float f);
	void firePositiveLaser(const Point3& targetPosition);
	void fireNegativeLaser(const Point3& targetPosition);
	void dropEbomb(const Point3& targetLocation); // attempts, won't drop if no bomb is available

	void onEvent(Collision_Player_Enemy&);
	void onEvent(Collision_Enemy_Laser&);
	void onEvent(Collision_Ebomb_Crater&);
	void onEvent(Collision_Ebomb_Terrain&);
	void onEvent(Player_EnergyGained&);
	void onEvent(Player_EnergyDrained&);
	void onEvent(Player_EnergyDispersed&);

private:

	float m_gamePlaneHeight; // Y coord of all gameplay things like laser, player, enemy

	// Player data
	int m_currentLives;
	int m_maxLives;
	Playership* m_playership;
	Playership* m_playershipPrototype;

	// Ebomb data
	EbombType m_currentEbomb; // UNKNOWN if not created
	int m_normalBombEnergy; // defaults to playership->energyCapacity
	int m_combinedBombEnergy; // defaults to playership->energyCapacity / 2
	Ebomb* m_ebombPrototype;
	float m_ebombInitialDownwardVelocity;


	// Gameplay objects
	typedef std::list<Enemyship*> EnemyshipList;
	typedef std::list<Spawnpoint*> SpawnpointList;
	typedef std::list<Crater*> CraterList;
	typedef std::list<Ebomb*> EbombList;
	typedef std::list<Laser*> LaserList;

	Terrain* m_terrain;
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
	EbombType _seeIfPlayerCanCreateEbombAndReturnTypeOfBomb();
	bool _checkNormalEbombCreation(EnergyType);
	bool _checkCombinedEbombCreation(EnergyType, EnergyType);
	void _checkEbombUncreation();
	void _fireLaser(const Point3& targetPosition, int type);
	template< typename T, typename EventType >
	void _cleanUpList( std::list<T*>& list ); // deletes objects with isToBeDeleted() == true
	void _deleteLevelData();

	// Singleton stuff
	friend Singleton< GameLogic >;
	GameLogic();
	~GameLogic();
};
