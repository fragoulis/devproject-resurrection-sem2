//*****************************************************************************
//**
//**   File:               Crater.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
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