#pragma once
#include "../Spaceship.h"
#include "../EnergyTypes.h"

class Enemyship : public Spaceship
{
public:
	Enemyship();
	Enemyship(int type) : m_type(type) { }
	virtual ~Enemyship();

	int getType() const { return m_type; }
	EnergyType getEnergyType() const { return m_energyType; }
	int getCollisionPower() const { return m_collisionPower; }

private:
	int m_type;
	EnergyType m_energyType;
	int m_collisionPower;
};
