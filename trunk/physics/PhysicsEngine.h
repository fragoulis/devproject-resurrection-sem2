//*****************************************************************************
//**
//**   File:               PhysicsEngine.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************


#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
#include "../utility/EventManager.h"
#include "../math/Vector3.h"
#include "Pusher.h"
#include "Spring.h"
#include <list>
class Terrain;
class Playership;
class Enemyship;
class Movable;
class Rigidbody;
class Spaceship;
class Laser;
class Ebomb;
class Crater;


/**
 * Does two things separately:
 *   move objects according to their physics
 *   do collision detection
 *
 * Both are implemented as easy and fast as possible.
 *
 *
 */
class PhysicsEngine :
	public Singleton< PhysicsEngine >,
	public EventListener< Terrain_Changed >,
	public EventListener< Player_Spawned >,
	public EventListener< Player_Despawned >,
	public EventListener< Enemy_Spawned >,
	public EventListener< Enemy_Despawned >,
	public EventListener< Ebomb_Spawned >,
	public EventListener< Ebomb_Despawned >,
	public EventListener< Crater_Spawned >,
	public EventListener< Crater_Despawned >,
	public EventListener< Laser_Spawned >,
	public EventListener< Laser_Despawned >,
	public EventListener< Level_Unload >,
    public EventListener< Interceptor_Clamped >
{
public:

	//! Called by Application startup.
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void onEvent(Terrain_Changed&);
	void onEvent(Player_Spawned&);
	void onEvent(Player_Despawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Despawned&);
	void onEvent(Ebomb_Spawned&);
	void onEvent(Ebomb_Despawned&);
	void onEvent(Crater_Spawned&);
	void onEvent(Crater_Despawned&);
	void onEvent(Laser_Spawned&);
	void onEvent(Laser_Despawned&);
	void onEvent(Level_Unload&);
    void onEvent(Interceptor_Clamped&);

	void update(float dt);

private:
	Terrain* m_terrain;

	// Physics: forces, movement and such
    typedef std::list<Pusher> PusherList;
    typedef std::list<Spring> SpringList;
	typedef std::list<Movable*> MovableList;
	typedef std::list<Rigidbody*> RigidbodyList;
	typedef std::list<Spaceship*> SpaceshipList;

    SpringList m_springs;
    PusherList m_pushers;
	MovableList m_movables;
	RigidbodyList m_rigidbodies;
	SpaceshipList m_spaceships;

	void _updatePhysics(float dt);
	void _updateMovable(Movable* m, float dt);
	void _updateRigidbody(Rigidbody* r, float dt);
	void _updateSpaceship(Spaceship* s, float dt);
    
    inline void _addPusher( pusher_obj_t *a, pusher_obj_t *b, float restLength );
    inline void _addSpring( spring_obj_t *a, spring_obj_t *b, float restLength );

    void _updatePushers();
    void _updateSprings();

	void _getRigidbodyForcesAndMoments(Rigidbody* r, Vector3& forces, Vector3& moments);
	void _getSpaceshipForcesAndMoments(Spaceship* s, Vector3& forces, Vector3& moments);

	template< typename T, typename ForcesAndMomentsFunction >
	void _integrateForcesAndMoments(T* t, ForcesAndMomentsFunction f, float dt);

    // The minimum distance the planes must keep from each other
    float m_minDistanceBetweenShips;

	// Collision detection
	typedef std::list<Enemyship*> EnemyshipList;
	typedef std::list<Laser*> LaserList;
	typedef std::list<Ebomb*> EbombList;
	typedef std::list<Crater*> CraterList;

	float m_terrainHeight;
	Playership* m_playership;
	EnemyshipList m_enemyships;
	LaserList m_lasers;
	EbombList m_ebombs;
	CraterList m_craters;

	void _checkCollisions();
	void _checkPlayerEnemyCollisions();
	void _checkEnemyLaserCollisions();
	void _checkEbombCraterCollisions();
	void _checkEbombTerrainCollisions();



	friend Singleton< PhysicsEngine >;
	PhysicsEngine();
	virtual ~PhysicsEngine();
};

void PhysicsEngine::_addPusher( pusher_obj_t *a, pusher_obj_t *b, float restLength ) {
    m_pushers.push_back(
        Pusher( a, b, restLength )
        );
}

void PhysicsEngine::_addSpring( spring_obj_t *a, spring_obj_t *b, float restLength ) {
    m_springs.push_back(
        Spring( a, b, restLength )
        );
}