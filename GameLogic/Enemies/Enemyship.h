#pragma once
#include "../Spaceship.h"
#include "../EnergyTypes.h"
class ParserSection;

/**
 * Stores type, energy type, collision power and hit points.
 */
class Enemyship : public Spaceship
{
public:
	Enemyship();
	Enemyship(int type) : m_type(type) { }
	virtual ~Enemyship();

	int getType() const { return m_type; }
	EnergyType getEnergyType() const { return m_energyType; }
	int getCollisionPower() const { return m_collisionPower; }

	void loadSettings(const ParserSection&);

private:
	int m_type;
	EnergyType m_energyType;
	int m_collisionPower;
	int m_hitpoints;
};
