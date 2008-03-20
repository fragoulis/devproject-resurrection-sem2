//*****************************************************************************
//**
//**   File:               PhysicsEvents.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************


#include "../math/Vector3.h"
#include "../math/Point3.h"
class Playership;
class Enemyship;
class Laser;


/**
 * Collision event is a special templated class to make it easier to code the PhysicsEngine
 * Down below is a list of typedefs for specific events.
 * The templated version should not be used directly, but the typedefs isntead.
 */
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
typedef Collision<Enemyship, Laser> Collision_Enemy_Laser;
