#pragma once
#include "Rigidbody.h"

class Thruster
{
public:
	const Point3 getPosition() const { return m_position; }
	float getPower() const { return m_power; }

	void setPosition(const Point3& p) { m_position = p; }
	void setPower(float p) { m_power = p; }

private:
	Point3 m_position;
	float m_power;
};


class Spaceship : public Rigidbody
{
public:
	Spaceship();
	virtual ~Spaceship();

	float getThrusterFactor() const { return m_thrusterFactor; }
	void setThrusterFactor(float f) { m_thrusterFactor = f; }

	const Thruster& getLeftThruster() const { return m_leftThruster; }
	const Thruster& getRightThruster() const { return m_rightThruster; }
	void setLeftThruster(const Thruster& t) { m_leftThruster = t; }
	void setRightThruster(const Thruster& t) { m_rightThruster = t; }

private:

	float m_thrusterFactor;
	Thruster m_leftThruster, m_rightThruster;
};
