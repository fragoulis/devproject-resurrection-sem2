#pragma once
#include "WorldObject.h"


/**
 * Extends WorldObject by giving it a velocity and angularVelocity.
 * Sweet for Lasers.
 */
class Movable : public WorldObject
{
public:
	Movable();
	virtual ~Movable();

	const Vector3& getVelocity() const { return m_velocity; }
	void setVelocity(const Vector3& v) { m_velocity = v; }

	const Vector3& getAngularVelocity() const { return m_angularVelocity; }
	void setAngularVelocity(const Vector3& v) { m_angularVelocity = v; }


private:
	Vector3 m_velocity;
	Vector3 m_angularVelocity;
};
