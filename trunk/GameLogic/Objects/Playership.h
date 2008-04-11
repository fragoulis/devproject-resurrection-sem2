//*****************************************************************************
//**
//**   File:               Playership.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


#pragma once
#include "../Spaceship.h"
#include "../EnergyTypes.h"
#include "../Buffs/BuffContainer.h"
class Point3;

class Playership : public Spaceship
{
public:
	Playership();
	virtual ~Playership();

	void update(float dt);
	void confine(const Point3& minPoint, const Point3& maxPoint);

	int getEnergy(EnergyType type) const { return m_energy[type]; }
	void setEnergy(EnergyType type, int amount) { m_energy[type] = amount; }

	int getEnergyCapacity() const { return m_energyCapacity; }
	int getTotalEnergy();

	void resetAllEnergy();
	void respawn();

	bool isInvulnerable() const { return m_state == INVULNERABLE; }

	void addBuff(int type) { m_buffContainer.addBuff(type); }
	bool hasBuff(int type) { return m_buffContainer.hasBuff(type); }
	int getBuffStacks(int type) { return m_buffContainer.getBuffStacks(type); }

	void loadSettings(const ParserSection&);

private:
	enum State {
		ALIVE,
		DYING, // not used yet
		INVULNERABLE, // set when player restarts
		TO_BE_DELETED,
		UNKNOWN,
	};

	State m_state;
	float m_timeTillNextEvent;
	float m_respawnInvulnerableTime;
	int m_energy[ENERGY_TYPE_COUNT];
	int m_energyCapacity;
	BuffContainer m_buffContainer;
};
