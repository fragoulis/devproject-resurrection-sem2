#pragma once
#include "../Movable.h"
#include "LaserTypes.h"

class Laser : public Movable
{
public:
	Laser();
	virtual ~Laser();

	LaserType getLaserType() const { return m_type; }
	bool isDead() const { return m_age > m_lifetime; }

	void update(float dt) { m_age += dt; }

private:
	LaserType m_type;
	float m_age;
	float m_lifetime;
};
