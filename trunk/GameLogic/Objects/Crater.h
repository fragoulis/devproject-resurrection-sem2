//*****************************************************************************
//**
//**   File:               Crater.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../WorldObject.h"
#include "../EnergyTypes.h"
class ParserSection;

class Crater : public WorldObject
{
public:
	Crater();
	virtual ~Crater();

	void loadSettings(const ParserSection&);

private:
	float m_radius;
	EnergyType m_energyType;
};
