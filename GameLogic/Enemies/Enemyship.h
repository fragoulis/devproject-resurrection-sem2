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
	Enemyship(int type) { setType(type); }
	virtual ~Enemyship();

	EnergyType getEnergyType() const { return m_energyType; }
	int getCollisionPower() const { return m_collisionPower; }
	int getHitPoints() const { return m_hitpoints; }

	void loadSettings(const ParserSection&);

private:
	EnergyType m_energyType;
	int m_collisionPower;
	int m_hitpoints;
};
