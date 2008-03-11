//*****************************************************************************
//**
//**   File:               PhysicsEngine.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
#include "../utility/EventManager.h"
#include "../math/Vector3.h"
#include <list>
class Terrain;
class Playership;
class Enemyship;
class Movable;
class Rigidbody;
class Spaceship;


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
	public EventListener< Enemy_Despawned >
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

	void update(float dt);

private:
	Terrain* m_terrain;

	// Physics: forces, movement and such
	typedef std::list<Movable*> MovableList;
	typedef std::list<Rigidbody*> RigidbodyList;
	typedef std::list<Spaceship*> SpaceshipList;

	MovableList m_movables;
	RigidbodyList m_rigidbodies;
	SpaceshipList m_spaceships;

	void updatePhysics(float dt);
	void updateMovable(Movable* m, float dt);
	void updateRigidbody(Rigidbody* r, float dt);
	void updateSpaceship(Spaceship* s, float dt);

	void getRigidbodyForcesAndMoments(Rigidbody* r, Vector3& forces, Vector3& moments);
	void getSpaceshipForcesAndMoments(Spaceship* s, Vector3& forces, Vector3& moments);

	// can't be arsed to write the type of ForcesAndMomentsFunction :)
	template< typename T, typename ForcesAndMomentsFunction >
	void integrateForcesAndMoments(T* t, ForcesAndMomentsFunction f, float dt);



	// Collision detection
	typedef std::list<Enemyship*> EnemyshipList;
	typedef std::list<Playership*> PlayershipList;

	PlayershipList m_playerships;
	EnemyshipList m_enemyships;

	void checkCollisions();

	template< typename T1, typename T2 >
	void checkCircleCollisions(std::list<T1*>& list1, std::list<T2*>& list2);



	friend Singleton< PhysicsEngine >;
	PhysicsEngine();
	virtual ~PhysicsEngine();
};