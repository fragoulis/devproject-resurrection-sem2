#ifndef _RES_GAME_LOGIC_H_
#define _RES_GAME_LOGIC_H_
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include "../utility/TimerManager.h"
#include "../physics/PhysicsEvents.h"
#include "GameEvents.h"
#include "EbombTypes.h"
#include <string>
#include <vector>
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
 *   etc.
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
	public EventListener< Player_EnergyDispersed >,
	public EventListener< Player_Destroyed >,
	public EventListener< Level_Complete >
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


	void loadLevel(const std::string& levelName);
	void unloadLevel();
	const std::string& getCurrentLevel() const { return m_levelName; }

	// These are functions a controller can call to change the game state
	Enemyship* spawnEnemy(int type, EnergyType energyType, float x, float z, bool force);
	void setPlayerDirection(const Vector3& v);
	void setPlayerThrusterPower(float f);
	void firePositiveLaser(const Point3& targetPosition);
	void fireNegativeLaser(const Point3& targetPosition);
	void dropEbomb(const Point3& targetLocation); // attempts, won't drop if no bomb is available
	void swapLasers();
	void unSwapLasers();


	// events!
	void onEvent(Collision_Player_Enemy&);
	void onEvent(Collision_Enemy_Laser&);
	void onEvent(Collision_Ebomb_Crater&);
	void onEvent(Collision_Ebomb_Terrain&);
	void onEvent(Player_EnergyGained&);
	void onEvent(Player_EnergyDrained&);
	void onEvent(Player_EnergyDispersed&);
	void onEvent(Player_Destroyed&);
	void onEvent(Level_Complete&);

private:

	// some random stuff
	std::string m_levelName;
	float m_gamePlaneHeight; // Y coord of all gameplay things like laser, player, enemy
	TimerManager m_timerManager;

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

	int m_maxEnemies;


	// Gameplay objects
	typedef std::vector<Enemyship*> EnemyshipList;
	typedef std::vector<Spawnpoint*> SpawnpointList;
	typedef std::vector<Crater*> CraterList;
	typedef std::vector<Ebomb*> EbombList;
	typedef std::vector<Laser*> LaserList;
	typedef EnemyshipList::iterator EnemyshipListIt;

	Terrain* m_terrain;
	EnemyshipList m_enemyships;
	SpawnpointList m_spawnpoints;
	CraterList m_craters;
	EbombList m_ebombs;
	LaserList m_lasers;

	// Player laser firing data
	float m_playerLaserCooldownTime;
	float m_playerLaserCooldownLeft;
	float m_laserStartOffset;
	int m_laserTypePositive;
	int m_laserTypeNegative;
	int m_laserSwapDebuffType;
	int m_laserPowerType;//[ENERGY_TYPE_COUNT];
	float m_laserPowerFactor;
	bool m_lasersSwapped;

	// helper functions
	//void _addLaserPowerBuffs(EbombType);
	EbombType _seeIfPlayerCanCreateEbombAndReturnTypeOfBomb();
	bool _checkNormalEbombCreation(EnergyType);
	bool _checkCombinedEbombCreation(EnergyType, EnergyType);
	void _checkEbombUncreation();
	void _fireLaser(const Point3& targetPosition, int type);
	template< typename T, typename EventType >
	void _cleanUpList( std::list<T*>& list ); // deletes objects with isToBeDeleted() == true
	template< typename T, typename EventType >
	void _cleanUpVector( std::vector<T*>& vec ); // deletes objects with isToBeDeleted() == true
	void _deleteLevelData();

	// Player destroyed
	void _playerDestroyed1_DespawnEnemies();
	void _playerDestroyed2_RespawnPlayer();

	// Level Complete
	void _levelComplete1_DestroyEnemies();


	// Singleton stuff
	friend Singleton< GameLogic >;
	GameLogic();
	~GameLogic();
};



#endif