//*****************************************************************************
//**
//**   File:               Spaceship.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "Rigidbody.h"
class ParserSection;

/**
 * Spaceship extends Rigidbody by giving it a thruster.
 * It also adds a radius as we are assuming all spaceships are circles.
 */


struct ThrusterData
{
	float factor;
	float power;
	float maxPower;
	Vector3 direction;
};


class Spaceship : public Rigidbody
{
public:
	Spaceship();
	virtual ~Spaceship();

	const ThrusterData& getThrusterData() const { return m_thrusterData; }
	void setThrusterData(const ThrusterData& td) { m_thrusterData = td; }

	const Vector3& getThrusterDirection() const { return m_thrusterData.direction; }
	void setThrusterDirection(const Vector3& d) { m_thrusterData.direction = d; }

	float getThrusterFactor() const { return m_thrusterData.factor; }
	void setThrusterFactor(float f) { m_thrusterData.factor = f; }

	float getThrusterPower() const { return m_thrusterData.power; }
	void setThrusterPower(float p) { m_thrusterData.power = p; }

	float getMaxThrusterPower() const { return m_thrusterData.maxPower; }
	void setMaxThrusterPower(float p) { m_thrusterData.maxPower = p; }

	float getRadius() const { return m_radius; }
	void setRadius(float r) { m_radius = r; }

	void loadSettings(const ParserSection& ps);

private:
	ThrusterData m_thrusterData;
	float m_radius;
};
