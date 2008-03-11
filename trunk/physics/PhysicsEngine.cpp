//*****************************************************************************
//**
//**   File:               PhysicsEngine.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "PhysicsEngine.h"
#include "PhysicsEvents.h"
#include "../GameLogic/Movable.h"
#include "../GameLogic/Rigidbody.h"
#include "../GameLogic/Spaceship.h"
#include "../GameLogic/Objects/Playership.h"   // needed to convert to Spaceship*
#include "../GameLogic/Enemies/Enemyship.h"    // needed to convert to Spaceship*
#include "../math/maths.h"
#include <iostream>
using namespace std;

const float EARTH_GRAVITY = 9.81f;

PhysicsEngine :: PhysicsEngine()
{
}

PhysicsEngine :: ~PhysicsEngine()
{
	onApplicationUnload();
}

void PhysicsEngine :: onApplicationLoad( const ParserSection& ps )
{
	EventManager::instance().registerEventListener< Terrain_Changed >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
}

void PhysicsEngine :: onApplicationUnload()
{
}


void PhysicsEngine :: onEvent( Terrain_Changed& evt )
{
	m_terrain = evt.getValue();
}

void PhysicsEngine :: onEvent( Player_Spawned& evt )
{
	m_spaceships.push_back(evt.getValue());
}

void PhysicsEngine :: onEvent( Enemy_Spawned& evt )
{
	//m_spaceships.push_back(evt.getValue());
}



void PhysicsEngine :: update( float dt )
{
	updatePhysics(dt);
	checkCollisions();
}

void PhysicsEngine :: updatePhysics( float dt )
{
	for (MovableList::iterator i = m_movables.begin(); i != m_movables.end(); ++i)
	{
		updateMovable(*i, dt);
	}

	for (RigidbodyList::iterator i = m_rigidbodies.begin(); i != m_rigidbodies.end(); ++i)
	{
		updateRigidbody(*i, dt);
	}

	for (SpaceshipList::iterator i = m_spaceships.begin(); i != m_spaceships.end(); ++i)
	{
		updateSpaceship(*i, dt);
	}
}

void PhysicsEngine :: updateMovable( Movable* m, float dt )
{
	//Point3 newpos = m->getPosition() + m->getVelocity() * dt;
	//cout << "OldPos " << m->getPosition() << " Newpos " << newpos << " velocity " << m->getVelocity() << " dt " << dt << endl;
	m->setPosition(m->getPosition() + m->getVelocity() * dt);
}

void PhysicsEngine :: updateRigidbody( Rigidbody* r, float dt )
{
	integrateForcesAndMoments(r, &PhysicsEngine::getRigidbodyForcesAndMoments, dt);
}

void PhysicsEngine :: updateSpaceship( Spaceship* s, float dt )
{
	integrateForcesAndMoments(s, &PhysicsEngine::getSpaceshipForcesAndMoments, dt);

	// to rotate our ships, we simply rotate by a small factor toward the target direction
	// this means we start moving before our ship faces the correct direction
	// only 2D physics, so should be easy!
	// have to find the 2D angle, change it a bit, use cos/sin to get new X/Y
	// how do I get the direction from a Quaternion? grr
}

template< typename T, typename ForcesAndMomentsFunction >
void PhysicsEngine :: integrateForcesAndMoments(T* t, ForcesAndMomentsFunction f, float dt)
{
	Vector3 forces, moments;
	forces.set(0.0f, 0.0f, 0.0f);
	moments.set(0.0f, 0.0f, 0.0f);
	(this->*f)(t, forces, moments);
	Vector3 acceleration = forces / t->getMass() * dt;
	t->setVelocity(t->getVelocity() + acceleration);
	updateMovable(t, dt);
}


void PhysicsEngine :: getRigidbodyForcesAndMoments( Rigidbody* r, Vector3& forces, Vector3& moments )
{
	// Gravity, we assume Z points upward
	forces.setY(forces.getY() - r->getGravityData().factor * EARTH_GRAVITY * r->getMass());

	// Simple airdrag, assumes turbulent flow
	const AirdragData& ad = r->getAirdragData();
	const Vector3& vel = r->getVelocity();
	if (!Math::float_is_zero(vel.lengthSquared())) {
		Vector3 airdrag_direction = -vel;
		airdrag_direction.normalize();
		float speed = r->getVelocity().length();
		forces += airdrag_direction * ad.factor * ad.coefficient * speed * speed;
	}

	// Simple lift
	forces.setY(forces.getY() + r->getLiftData().factor * EARTH_GRAVITY * r->getMass());
}

void PhysicsEngine :: getSpaceshipForcesAndMoments( Spaceship* s, Vector3& forces, Vector3& moments )
{
	// get rigid body forces and moments
	getRigidbodyForcesAndMoments(s, forces, moments);

	// add force pointing in target direction
	ThrusterData td = s->getThrusterData();
	if (!Math::float_is_zero(td.direction.lengthSquared())) {
		forces += td.factor * td.power * td.direction;
	}
}



void PhysicsEngine :: checkCollisions()
{
	checkCircleCollisions(m_playerships, m_enemyships);
}

template< typename T1, typename T2 >
void PhysicsEngine :: checkCircleCollisions(std::list<T1*>& list1, std::list<T2*>& list2)
{
	typedef std::list<T1*> List1;
	typedef std::list<T2*> List2;
	
	for (List1::iterator i = list1.begin(); i != list1.end(); ++i)
	{
		T1* t1 = *i;
		Point3 pos1 = t1->getPosition();
		pos1.setY(0.0f);
		float r1 = t1->getRadius();
		for (List2::iterator j = list2.begin(); j != list2.end(); ++j)
		{
			T2* t2 = *j;
			Point3 pos2 = t2->getPosition();
			pos2.setY(0.0f);
			float r2 = t2->getRadius();
			float distance = pos1.distance(pos2) - (r1 + r2);
			if (distance < 0.0f) {
				Vector3 normal = pos1 - pos2;
				normal.normalize();
				Point3 colpos = pos1 + ((r1 + distance / 2) * normal);
				EventManager::instance().fireEvent(Collision<T1, T2>(t1, t2, colpos, normal));
			}
		}
	}
}