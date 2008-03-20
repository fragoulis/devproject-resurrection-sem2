//*****************************************************************************
//**
//**   File:               Crater.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
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
