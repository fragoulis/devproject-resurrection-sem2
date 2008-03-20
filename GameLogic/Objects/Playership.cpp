//*****************************************************************************
//**
//**   File:               Playership.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Playership.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"

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
}
