#include "../math/Vector3.h"
#include "../math/Point3.h"
class Playership;
class Enemyship;


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
