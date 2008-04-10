//*****************************************************************************
//**
//**   File:               Playership.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


#include "Playership.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../WorldObjectTypeManager.h"
#include "../../math/Point3.h"

Playership :: Playership()
{
	resetAllEnergy();
	m_state = ALIVE;
}

Playership :: ~Playership()
{
}

void Playership :: update(float dt)
{
	m_buffContainer.update(dt, this);
	m_timeTillNextEvent -= dt;
	switch (m_state) {
		case INVULNERABLE :
		{
			if (m_timeTillNextEvent < 0.0f) {
				m_state = ALIVE;
			}
			break;
		}
	}
}

void Playership :: confine(const Point3& minPoint, const Point3& maxPoint)
{
	const Point3& pos = getPosition();
	setPosition(Point3(
		min(maxPoint.getX(), max(minPoint.getX(), pos.getX())),
		min(maxPoint.getY(), max(minPoint.getY(), pos.getY())),
		min(maxPoint.getZ(), max(minPoint.getZ(), pos.getZ())))
	);
}

int Playership :: getTotalEnergy()
{
	int total = 0;
	for (int i = 0; i < ENERGY_TYPE_COUNT; i++) total += m_energy[i];
	return total;
}

void Playership :: resetAllEnergy()
{
	for (int i = 0; i < ENERGY_TYPE_COUNT; i++)
		m_energy[i] = 0;
}

void Playership :: respawn()
{
	resetAllEnergy();
	m_state = INVULNERABLE;
	m_timeTillNextEvent = m_respawnInvulnerableTime;
	m_buffContainer.removeAll();
}

void Playership :: loadSettings(const ParserSection& ps)
{
	m_energyCapacity = FromString<int>(ps.getVal("EnergyCapacity"));
	m_respawnInvulnerableTime = FromString<float>(ps.getVal("RespawnInvulnerableTime"));
	Spaceship::loadSettings(ps);
	setType(WorldObjectTypeManager::instance().getTypeFromName("PlayerShip"));
}
