//*****************************************************************************
//**
//**   File:               Playership.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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

	void loadSettings(const ParserSection&);

private:
	int m_energy[ENERGY_TYPE_COUNT];
	int m_energyCapacity;
};
