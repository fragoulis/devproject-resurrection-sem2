#pragma once
#include "../utility/Singleton.h"
#include "../GameLogic/GameLogic.h"
#include "../utility/EventManager.h"
#include "../math/Vector3.h"
#include "../math/Point3.h"
#include <list>
class Terrain;
class Playership;
class Enemyship;
class Movable;
class Rigidbody;
class Spaceship;


// Collision event
template< typename T1, typename T2 >
class Collision
{
public:
	Collision(T1* t1, T2* t2, Point3 point, Vector3 normal) :
	  m_t1(t1), m_t2(t2), m_point(point), m_normal(normal) { }

	T1* getObject1() { return m_t1; }
	T2* getObject2() { return m_t2; }
	const Point3& getPoint() const { return m_point; }
	const Vector3& getNormal() const { return m_normal; } // always points away from t2, towards t1

private:
	T1* m_t1;
	T2* m_t2;
	Point3 m_point;
	Vector3 m_normal;
};

// a list of typedefs for collision events
typedef Collision<Playership, Enemyship> Collision_Player_Enemy;




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
};