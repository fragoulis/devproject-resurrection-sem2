#pragma once
#include "Rigidbody.h"


struct ThrusterData
{
	float factor;
	float power;
	Vector3 direction;
	float rotationSpeed;
};



class Spaceship : public Rigidbody
{
public:
	Spaceship();
	virtual ~Spaceship();


	const ThrusterData& getThrusterData() const { return m_thrusterData; }
	void setThrusterData(ThrusterData& td) { m_thrusterData = td; }


	//float getThrusterFactor() const { return m_thrusterFactor; }
	//void setThrusterFactor(float f) { m_thrusterFactor = f; }

	//float getThrusterPower() const { return m_thrusterPower; }
	//void setThrusterPower(float f) { m_thrusterPower = f; }

	//const Vector3& getTargetDirection() const { return m_targetDirection; }
	//void setTargetDirection(const Vector3& t) { m_targetDirection = t; }

	
	//const Thruster& getLeftThruster() const { return m_leftThruster; }
	//const Thruster& getRightThruster() const { return m_rightThruster; }
	//void setLeftThruster(const Thruster& t) { m_leftThruster = t; }
	//void setRightThruster(const Thruster& t) { m_rightThruster = t; }

private:

	ThrusterData m_thrusterData;
};
