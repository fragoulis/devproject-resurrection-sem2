//*****************************************************************************
//**
//**   File:               Crater.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Crater.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"

Crater :: Crater()
{
}

Crater :: ~Crater()
{
}

void Crater :: loadSettings(const ParserSection& ps)
{
	m_radius = FromString<float>(ps.getVal("Radius"));
	m_energyType = EnergyTypeFromString(ps.getVal("EnergyType"));
	WorldObject::loadSettings(ps);
}