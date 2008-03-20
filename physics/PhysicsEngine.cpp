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
#include "../GameLogic/Lasers/Laser.h"
#include "../math/maths.h"
#include "../math/Vector3.h"
#include "../math/Point3.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include <iostream>
using namespace std;

const float EARTH_GRAVITY = 9.81f;
const float MIN_DT = 0.005f;

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
	EventManager::instance().registerEventListener< Player_Despawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);
	EventManager::instance().registerEventListener< Laser_Spawned >(this);
	EventManager::instance().registerEventListener< Laser_Despawned >(this);
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
	m_playership = evt.getValue();
	m_spaceships.push_back(evt.getValue());
}
void PhysicsEngine :: onEvent( Player_Despawned& evt )
{
	m_playership = 0;
	m_spaceships.remove(evt.getValue());
}

void PhysicsEngine :: onEvent( Enemy_Spawned& evt )
{
	m_enemyships.push_back(evt.getValue());
	m_spaceships.push_back(evt.getValue());
}

void PhysicsEngine :: onEvent( Enemy_Despawned& evt )
{
	m_enemyships.remove(evt.getValue());
	m_spaceships.remove(evt.getValue());
}

void PhysicsEngine :: onEvent( Laser_Spawned& evt )
{
	m_lasers.push_back(evt.getValue());
}

void PhysicsEngine :: onEvent( Laser_Despawned& evt )
{
	m_lasers.remove(evt.getValue());
}




void PhysicsEngine :: update( float dt )
{
	while (dt > MIN_DT) {
		_updatePhysics(MIN_DT);
		dt -= MIN_DT;
	}
	_updatePhysics(dt);
	_checkCollisions();
}

void PhysicsEngine :: _updatePhysics( float dt )
{
	for (MovableList::iterator i = m_movables.begin(); i != m_movables.end(); ++i)
	{
		_updateMovable(*i, dt);
	}

	for (RigidbodyList::iterator i = m_rigidbodies.begin(); i != m_rigidbodies.end(); ++i)
	{
		_updateRigidbody(*i, dt);
	}

	for (SpaceshipList::iterator i = m_spaceships.begin(); i != m_spaceships.end(); ++i)
	{
		_updateSpaceship(*i, dt);
	}
}

void PhysicsEngine :: _updateMovable( Movable* m, float dt )
{
	//Point3 newpos = m->getPosition() + m->getVelocity() * dt;
	//cout << "OldPos " << m->getPosition() << " Newpos " << newpos << " velocity " << m->getVelocity() << " dt " << dt << endl;
	m->setPosition(m->getPosition() + m->getVelocity() * dt);
}

void PhysicsEngine :: _updateRigidbody( Rigidbody* r, float dt )
{
	_integrateForcesAndMoments(r, &PhysicsEngine::_getRigidbodyForcesAndMoments, dt);
}

void PhysicsEngine :: _updateSpaceship( Spaceship* s, float dt )
{
	_integrateForcesAndMoments(s, &PhysicsEngine::_getSpaceshipForcesAndMoments, dt);

	// this should rotate the ship 90 degrees around y-axis.
	// uncomment to have a look for yourself :)
	//Rotation r;
	//Vector3 axis(0.0f, 1.0f, 0.0f);
	//axis.normalize();
	//float angle = 3.1415f * 0.5f;
	//r.set(axis, angle);

	//s->setRotation(r);

	// to rotate our ships, we simply rotate by a small factor toward the target direction
	// this means we start moving before our ship faces the correct direction
	// only 2D physics, so should be easy!
	// have to find the 2D angle, change it a bit, use cos/sin to get new X/Y
	// how do I get the direction from a Quaternion? grr

	const Vector3& direction = s->getThrusterDirection();
	if (direction.lengthSquared() > 0.0f) {
		float angle = direction.angle(Vector3(0.0f, 0.0f, -1.0f));
		if (direction.dot(Vector3(1.0f, 0.0f, 0.0f)) < 0.0f) angle = -angle;
		Rotation r(Vector3(0.0f, 1.0f, 0.0f), angle);
		s->rotateTowards(r, 0.1f);
	}
}

template< typename T, typename ForcesAndMomentsFunction >
void PhysicsEngine :: _integrateForcesAndMoments(T* t, ForcesAndMomentsFunction f, float dt)
{
	Vector3 forces, moments;
	forces.set(0.0f, 0.0f, 0.0f);
	moments.set(0.0f, 0.0f, 0.0f);
	(this->*f)(t, forces, moments);
	Vector3 acceleration = forces / t->getMass() * dt;
	t->setVelocity(t->getVelocity() + acceleration);
	_updateMovable(t, dt);
}


void PhysicsEngine :: _getRigidbodyForcesAndMoments( Rigidbody* r, Vector3& forces, Vector3& moments )
{
	// Gravity, we assume Z points upward
	forces.setY(forces.getY() - r->getGravityData().factor * EARTH_GRAVITY * r->getMass());

	// Simple airdrag, assumes turbulent flow
	const AirdragData& ad = r->getAirdragData();
	const Vector3& vel = r->getVelocity();
	const float speed = vel.length();
	if (!Math::float_is_zero(speed)) {
		Vector3 airdrag_direction = -vel;
		airdrag_direction.normalize();
		forces += airdrag_direction * ad.factor * ad.coefficient * speed * speed;
	}

	// Simple lift
	forces.setY(forces.getY() + r->getLiftData().factor * EARTH_GRAVITY * r->getMass());
}

void PhysicsEngine :: _getSpaceshipForcesAndMoments( Spaceship* s, Vector3& forces, Vector3& moments )
{
	// get rigid body forces and moments
	_getRigidbodyForcesAndMoments(s, forces, moments);

	// add force pointing in target direction
	ThrusterData td = s->getThrusterData();
	if (!Math::float_is_zero(td.direction.lengthSquared())) {
		forces += td.factor * td.power * td.direction;
	}
}



void PhysicsEngine :: _checkCollisions()
{
	_checkPlayerEnemyCollisions();
	_checkEnemyLaserCollisions();
}


void PhysicsEngine :: _checkPlayerEnemyCollisions()
{
	Point3 pos1 = m_playership->getPosition();
	pos1.setY(0.0f);
	float r1 = m_playership->getRadius();
	for (EnemyshipList::iterator it = m_enemyships.begin(); it != m_enemyships.end(); ++it)
	{
		Enemyship* enemy = *it;
		if (enemy->isToBeDeleted()) continue;
		Point3 pos2 = enemy->getPosition();
		pos2.setY(0.0f);
		float r2 = enemy->getRadius();
		float distance = pos1.distance(pos2) - (r1 + r2);
		if (distance < 0.0f) {
			Vector3 normal = pos1 - pos2;
			normal.normalize();
			Point3 colpos = pos1 + ((r1 + distance / 2) * normal);
			CKLOG(std::string("Collision between player and enemy ") + ToString<Enemyship*>(enemy), 1);
			EventManager::instance().fireEvent(Collision_Player_Enemy(m_playership, enemy, colpos, normal));
		}
	}
}



void PhysicsEngine :: _checkEnemyLaserCollisions()
{
	for (EnemyshipList::iterator i = m_enemyships.begin(); i != m_enemyships.end(); ++i)
	{
		Enemyship* enemy = *i;
		if (enemy->isToBeDeleted()) continue;
		Point3 circleCentre = enemy->getPosition();
		float circleRadius = enemy->getRadius();

		for (LaserList::iterator j = m_lasers.begin(); j != m_lasers.end(); ++j)
		{
			Laser* laser = *j;
			if (laser->isToBeDeleted() || laser->isEnding()) continue;
			float totalRadiusSquared = circleRadius + laser->getWidth();
			totalRadiusSquared *= totalRadiusSquared;
			Point3 linePos1 = laser->getFrontPoint();
			Point3 linePos2 = laser->getBackPoint();
			Vector3 lineDirection = linePos2 - linePos1;
			Vector3 toPoint1 = circleCentre - linePos1;
			float t = Math::clamp(lineDirection.dot(toPoint1) / lineDirection.lengthSquared(), 0.0f, 1.0f);
			Point3 closestPointOnLine = linePos1 + t * lineDirection;
			float distanceSquared = (circleCentre - closestPointOnLine).lengthSquared();
			if (distanceSquared < totalRadiusSquared)
			{
				Vector3 normal = circleCentre - closestPointOnLine;
				normal.normalize();
				CKLOG(std::string("Collision between enemy ") + ToString<Enemyship*>(enemy) + " and laser " + ToString<Laser*>(laser), 1);
				EventManager::instance().fireEvent(Collision_Enemy_Laser(enemy, laser, closestPointOnLine, normal));
			}
		}
	}
}