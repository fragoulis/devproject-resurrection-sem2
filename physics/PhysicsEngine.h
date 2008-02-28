#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameLogic.h"
#include "../utility/EventManager.h"
#include <list>
class Terrain;
class Playership;
class Movable;
class Rigidbody;
class Spaceship;
class Vector3;


class PhysicsEngine :
	public Singleton< PhysicsEngine >,
	public EventListener< Terrain_Changed >,
	public EventListener< Player_Spawned >
{
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	void onEvent(Terrain_Changed&);
	void onEvent(Player_Spawned&);

	void update(float dt);

private:
	Terrain* m_terrain;

	typedef std::list<Movable*> MovableList;
	typedef std::list<Rigidbody*> RigidbodyList;
	typedef std::list<Spaceship*> SpaceshipList;

	MovableList m_movables;
	RigidbodyList m_rigidbodies;
	SpaceshipList m_spaceships;

	void updateMovable(Movable* m, float dt);
	void updateRigidbody(Rigidbody* r, float dt);
	void updateSpaceship(Spaceship* s, float dt);

	void getRigidbodyForcesAndMoments(Rigidbody* r, Vector3& forces, Vector3& moments);
	void getSpaceshipForcesAndMoments(Spaceship* s, Vector3& forces, Vector3& moments);

	// can't be arsed to write the type of ForcesAndMomentsFunction :)
	template< typename T, typename ForcesAndMomentsFunction >
	void integrateForcesAndMoments(T* t, ForcesAndMomentsFunction f, float dt);

	friend Singleton< PhysicsEngine >;
};
