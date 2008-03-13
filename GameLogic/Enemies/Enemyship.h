//*****************************************************************************
//**
//**   File:               Enemyship.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


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
	Enemyship(int type);
	virtual ~Enemyship();

	EnergyType getEnergyType() const { return m_energyType; }
	int getCollisionPower() const { return m_collisionPower; }
	int getHitPoints() const { return m_hitPoints; }
	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }
	int getEnergyPoints() const { return m_energyPoints; }

	void setEnergyType(EnergyType et) { m_energyType = et; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }
	void reduceHitPoints(int amount) { m_hitPoints -= amount; }

	void loadSettings(const ParserSection&);

private:
	enum State {
		SPAWNING, // not used yet
		ALIVE,
		DYING, // not used yet
		TO_BE_DELETED,
		UNKNOWN,
	};

	EnergyType m_energyType;
	State m_state;
	int m_collisionPower;
	int m_hitPoints;
	int m_energyPoints;
};
