//*****************************************************************************
//**
//**   File:               Enemyship.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************


#pragma once
#include "../Spaceship.h"
#include "../EnergyTypes.h"
class ParserSection;
class Playership;

/**
 * Stores type, energy type, collision power and hit points.
 */
class Enemyship : public Spaceship
{
public:
	Enemyship(int type);
	virtual ~Enemyship();

	EnergyType getEnergyType() const { return m_energyType; }
	int getCollisionPower() const { return m_collisionPower; }
	int getHitPoints() const { return int(m_hitPoints); }
	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }
    bool isClamped() const { return m_state == ALIVE_CLAMPED; }
	int getEnergyPoints() const { return m_energyPoints; }

	void setEnergyType(EnergyType et) { m_energyType = et; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }
    void setClamped() { m_state = ALIVE_CLAMPED; }
	void reduceHitPoints(int amount) { m_hitPoints -= float(amount); }
	void reduceHitPoints(float amount) { m_hitPoints -= amount; }

	virtual void collideWithPlayer(Playership* player);
	virtual void loadSettings(const ParserSection&);
	virtual void update(float dt);
	virtual Enemyship* clone();

private:
	enum State {
		SPAWNING, // not used yet
		ALIVE,
        ALIVE_CLAMPED,
		DYING, // not used yet
		TO_BE_DELETED,
		UNKNOWN,
	};

	EnergyType m_energyType;
	State m_state;
	int m_collisionPower;
	float m_hitPoints;
	int m_energyPoints;
};
