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

Playership :: Playership()
{
	resetAllEnergy();
}

Playership :: ~Playership()
{
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

void Playership :: loadSettings(const ParserSection& ps)
{
	m_energyCapacity = FromString<int>(ps.getVal("EnergyCapacity"));
	Spaceship::loadSettings(ps);
	setType(WorldObjectTypeManager::instance().getTypeFromName("PlayerShip"));
}
