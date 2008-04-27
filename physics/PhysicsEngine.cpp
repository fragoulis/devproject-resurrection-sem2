//*****************************************************************************
//**
//**   File:               PhysicsEngine.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


#include "PhysicsEngine.h"
#include "PhysicsEvents.h"
#include "../GameLogic/Movable.h"
#include "../GameLogic/Rigidbody.h"
#include "../GameLogic/Spaceship.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../GameLogic/Objects/Ebomb.h"
#include "../GameLogic/Objects/Crater.h"
#include "../GameLogic/Objects/Terrain.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../math/maths.h"
#include "../math/Vector3.h"
#include "../math/Point3.h"
#include "../math/Point2.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfxutils/Misc/utils.h"
#include <iostream>
using namespace std;

const float EARTH_GRAVITY = 200.0f;
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
	EventManager::instance().registerEventListener< Ebomb_Spawned >(this);
	EventManager::instance().registerEventListener< Ebomb_Despawned >(this);
	EventManager::instance().registerEventListener< Crater_Spawned >(this);
	EventManager::instance().registerEventListener< Crater_Despawned >(this);
	EventManager::instance().registerEventListener< Laser_Spawned >(this);
	EventManager::instance().registerEventListener< Laser_Despawned >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);
    EventManager::instance().registerEventListener< Interceptor_Clamped >(this);

    //const ParserSection* psGame = ps.getSection("main");
    //m_terrainHeight = FromString<float>(psGame->getVal("TerrainHeight"));

    const ParserSection* psPusher = ps.getSection("Physics:Pusher");
    m_pusher.Ks = FromString<float>(psPusher->getVal("Ks"));
    m_pusher.Kd = FromString<float>(psPusher->getVal("Kd"));
    m_pusher.minDistanceBetweenShips = FromString<float>(psPusher->getVal("MinDistanceBetweenShips"));

    const ParserSection* psSpring = ps.getSection("Physics:Spring");
    m_spring.Ks = FromString<float>(psSpring->getVal("Ks"));
    m_spring.Kd = FromString<float>(psSpring->getVal("Kd"));
    m_spring.minDistanceForClampedShips = FromString<float>(psSpring->getVal("MinDistanceForClampedShips"));
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
    m_springs.clear();
}

void PhysicsEngine :: onEvent( Enemy_Spawned& evt )
{
	Enemyship *enemyship = evt.getValue();
    int type = enemyship->getType();
	int carrierType = WorldObjectTypeManager::instance().getTypeFromName("EnemyCarrier");

	if (type != carrierType)
	{
		// Add springs between all enemies
        EnemyshipList::const_iterator it = m_enemyships.begin();
		for(; it != m_enemyships.end(); ++it )
		{
			Enemyship* es = *it;
			if( !es->isType(carrierType) )
			{
				_addPusher( enemyship, es );
			}
		}
	}

    m_enemyships.push_back(enemyship);
	m_spaceships.push_back(enemyship);
}

void PhysicsEngine :: onEvent( Enemy_Despawned& evt )
{
    Enemyship *enemyship = evt.getValue();
	m_enemyships.remove(enemyship);
	m_spaceships.remove(enemyship);
}

void PhysicsEngine :: onEvent( Ebomb_Spawned& evt )
{
	m_ebombs.push_back(evt.getValue());
	m_rigidbodies.push_back(evt.getValue());
}

void PhysicsEngine :: onEvent( Ebomb_Despawned& evt )
{
	m_ebombs.remove(evt.getValue());
	m_rigidbodies.remove(evt.getValue());
}

void PhysicsEngine :: onEvent( Crater_Spawned& evt )
{
	m_craters.push_back(evt.getValue());
}

void PhysicsEngine :: onEvent( Crater_Despawned& evt )
{
	m_craters.remove(evt.getValue());
}

void PhysicsEngine :: onEvent( Laser_Spawned& evt )
{
	m_lasers.push_back(evt.getValue());
}

void PhysicsEngine :: onEvent( Laser_Despawned& evt )
{
	m_lasers.remove(evt.getValue());
}

void PhysicsEngine :: onEvent(Level_Unload&)
{
	m_terrain = 0;
	m_pushers.clear();
    m_springs.clear();
	m_movables.clear();
	m_rigidbodies.clear();
	m_spaceships.clear();
	m_playership = 0;
	m_enemyships.clear();
	m_lasers.clear();
	m_ebombs.clear();
	m_craters.clear();
}

void PhysicsEngine :: onEvent(Interceptor_Clamped& evt)
{
    Enemyship *enemyship = evt.getValue();

    _addSpring( m_playership, enemyship );
}

void PhysicsEngine :: update( float dt )
{
	while (dt > MIN_DT) {
		_updatePhysics(MIN_DT);
		dt -= MIN_DT;
	}

    if( dt > 0.0f )
	    _updatePhysics(dt);

	_checkCollisions();
}

void PhysicsEngine :: _updatePhysics( float dt )
{
    _updatePushers();
    _updateSprings();

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

void PhysicsEngine :: _updatePushers()
{
    for( PusherList::iterator i = m_pushers.begin(); i != m_pushers.end(); )
    {
        Pusher &pusher= *i;
        const Enemyship *one = static_cast<const Enemyship*>(pusher.getFirstObject());
        const Enemyship *two = static_cast<const Enemyship*>(pusher.getSecondObject());
        if( one->getHitPoints() <= 0 || two->getHitPoints() <=0 )
        {
            // Remove the spring
            i = m_pushers.erase(i);
        } 
        else
        {
            pusher.compute();
            ++i;
        }
    }
}

void PhysicsEngine :: _updateSprings()
{
    for( SpringList::iterator i = m_springs.begin(); i != m_springs.end(); )
    {
        Spring &spring= *i;
        const Enemyship *two = static_cast<const Enemyship*>(spring.getSecondObject());
        if( two->getHitPoints() <=0 )
        {
            // Remove the spring
            i = m_springs.erase(i);
        } 
        else
        {
            spring.compute();
            ++i;
        }
    }
}

template< typename T, typename ForcesAndMomentsFunction >
void PhysicsEngine :: _integrateForcesAndMoments(T* t, ForcesAndMomentsFunction f, float dt)
{
	Vector3 moments;
    Vector3& forces = t->getForceAccum();
	moments.set(0.0f, 0.0f, 0.0f);
	(this->*f)(t, forces, moments);
	Vector3 acceleration = forces / t->getMass() * dt;
    forces.set(0.0f, 0.0f, 0.0f);
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
	forces.setY(forces.getY() + r->getLiftData().factor * r->getLiftData().power * EARTH_GRAVITY * r->getMass());
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
	_checkEbombCraterCollisions();
	_checkEbombTerrainCollisions();
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
			CKLOG(std::string("Collision between player and enemy ") + ToString<Enemyship*>(enemy), 3);
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
				CKLOG(std::string("Collision between enemy ") + ToString<Enemyship*>(enemy) + " and laser " + ToString<Laser*>(laser), 3);
				EventManager::instance().fireEvent(Collision_Enemy_Laser(enemy, laser, closestPointOnLine, normal));
			}
		}
	}
}


/**
 * Checks if any bomb hits a crater
 * Bomb is a sphere, crater a disc on the X-Z plane.
 */
void PhysicsEngine :: _checkEbombCraterCollisions()
{
	for (EbombList::iterator i = m_ebombs.begin(); i != m_ebombs.end(); ++i)
	{
		Ebomb* ebomb = *i;
		if (ebomb->isToBeDeleted()) continue;
		const Point3& sphereCentre = ebomb->getPosition();
		float sphereRadius = ebomb->getRadius();
		float sphereY = sphereCentre.getY();

		for (CraterList::iterator j = m_craters.begin(); j != m_craters.end(); ++j)
		{
			Crater* crater = *j;
			if (crater->isToBeDeleted()) continue;
			const Point3 craterPos = crater->getPosition();
			float craterY = craterPos.getY();
			float craterRadius = crater->getRadius();

			// first check if sphere touches the XZ plane at crater height at all
			if (sphereY - sphereRadius > craterY) continue;

			// (sphereY - craterY) / sphereRadius ranges from -1 to 1
			// we want it in range 0 to PI
			float y = (sphereY - craterY) / sphereRadius * float(Math::PI_2) + float(Math::PI_2);

			// now we got the radius of the circle on the plane
			// we can now do circle-circle collision
			float ebombCircleRadius = sin(y);
			Point2 craterCircleCentre(craterPos.getX(), craterPos.getZ());
			Point2 ebombCircleCentre(sphereCentre.getX(), sphereCentre.getZ());

			float distance = craterCircleCentre.distance(ebombCircleCentre);

			if (distance < (ebombCircleRadius + craterRadius))
			{
				Vector3 normal = sphereCentre - craterPos;
				normal.normalize();
				Point3 colpos = craterPos + ((craterRadius + distance / 2) * normal);
				CKLOG(std::string("Collision between ebomb ") + ToString<Ebomb*>(ebomb) + " and crater " + ToString<Crater*>(crater), 3);
				EventManager::instance().fireEvent(Collision_Ebomb_Crater(ebomb, crater, colpos, normal));
			}
		}
	}
}

/**
 * Checks if any bomb hits a crater
 * Bomb is a sphere, crater a disc on the X-Z plane.
 */
void PhysicsEngine :: _checkEbombTerrainCollisions()
{
	for (EbombList::iterator i = m_ebombs.begin(); i != m_ebombs.end(); ++i)
	{
		Ebomb* ebomb = *i;
		if (ebomb->isToBeDeleted()) continue;
		const Point3& sphereCentre = ebomb->getPosition();
		float sphereRadius = ebomb->getRadius();
		float sphereY = sphereCentre.getY();
		float terrainHeight = m_terrain->getHeight(sphereCentre.getX(), sphereCentre.getZ());

		if (sphereY < terrainHeight)
		{
			Vector3 normal(0.0f, 1.0f, 0.0f);
			Point3 colpos = sphereCentre;
			colpos.setY(terrainHeight);
			CKLOG(std::string("Collision between ebomb ") + ToString<Ebomb*>(ebomb) + " and terrain " + ToString<Terrain*>(m_terrain), 3);
			EventManager::instance().fireEvent(Collision_Ebomb_Terrain(ebomb, m_terrain, colpos, normal));
		}
	}
}
