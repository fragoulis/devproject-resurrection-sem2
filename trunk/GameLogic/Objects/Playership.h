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

class Playership : public Spaceship
{
public:
	Playership();
	virtual ~Playership();

	int getEnergy(EnergyType type) { return m_energy[type]; }
	void setEnergy(EnergyType type, int amount) { m_energy[type] = amount; }

	int getEnergyCapacity() { return m_energyCapacity; }
	int getTotalEnergy();

	void resetAllEnergy();

	void loadSettings(const ParserSection&);

private:
	int m_energy[ENERGY_TYPE_COUNT];
	int m_energyCapacity;
};
