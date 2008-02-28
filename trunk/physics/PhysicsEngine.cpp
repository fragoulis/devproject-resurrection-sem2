#include "PhysicsEngine.h"
#include "../GameLogic/Movable.h"
#include "../GameLogic/Rigidbody.h"
#include "../GameLogic/Spaceship.h"
#include "../GameLogic/Playership.h"

const float EARTH_GRAVITY = 9.81f;

PhysicsEngine :: PhysicsEngine()
{
}

PhysicsEngine :: ~PhysicsEngine()
{
}


void PhysicsEngine :: onEvent( Terrain_Changed& tc )
{
	m_terrain = tc.getData();
}

void PhysicsEngine :: onEvent( Player_Spawned& ps )
{
	m_spaceships.push_back(ps.getData());
}



void PhysicsEngine :: update( float dt )
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
	((*this).*f)(t, forces, moments);
	Vector3 acceleration = forces / t->getMass() * dt;
	t->setVelocity(t->getVelocity() + acceleration);
	updateMovable(t, dt);
}


void PhysicsEngine :: getRigidbodyForcesAndMoments( Rigidbody* r, Vector3& forces, Vector3& moments )
{
	// Gravity, we assume Z points upward
	forces.setZ(forces.getZ() + r->getGravityData().factor * EARTH_GRAVITY * r->getMass());

	// Simple airdrag, assumes turbulent flow
	const AirdragData& ad =r->getAirdragData();
	Vector3 airdrag_direction = -r->getVelocity();
	airdrag_direction.normalize();
	float speed = r->getVelocity().length();
	forces += airdrag_direction * ad.factor * ad.coefficient * speed * speed;

	// Simple lift
	forces.setZ(forces.getZ() + r->getLiftData().factor * EARTH_GRAVITY * r->getMass());
}

void PhysicsEngine :: getSpaceshipForcesAndMoments( Spaceship* s, Vector3& forces, Vector3& moments )
{
	// get rigid body forces and moments
	getRigidbodyForcesAndMoments(s, forces, moments);

	// add force pointing in target direction
	ThrusterData td = s->getThrusterData();
	forces += td.factor * td.power * td.direction;
}
