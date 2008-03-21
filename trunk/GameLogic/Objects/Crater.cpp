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
#include "../WorldObjectTypeManager.h"

Crater :: Crater()
{
	m_state = UNKNOWN;
	m_radius = 0.0f;
	m_ebombType = EBOMB_TYPE_UNKNOWN;
}

Crater :: ~Crater()
{
}

void Crater :: loadSettings(const ParserSection& ps)
{
	m_radius = FromString<float>(ps.getVal("Radius"));
	m_ebombType = EbombTypeFromString(ps.getVal("EbombType"));
	WorldObject::loadSettings(ps);

	setType(WorldObjectTypeManager::instance().getTypeFromName("Crater"));
}