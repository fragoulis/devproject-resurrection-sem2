#pragma once
#include "Rigidbody.h"

struct ThrusterData
{
	float factor;
	float power;
	Vector3 direction;
};


class Spaceship : public Rigidbody
{
public:
	Spaceship();
	virtual ~Spaceship();

	const ThrusterData& getThrusterData() const { return m_thrusterData; }
	void setThrusterData(const ThrusterData& td) { m_thrusterData = td; }


private:
	ThrusterData m_thrusterData;
};
