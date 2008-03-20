//*****************************************************************************
//**
//**   File:               Ebomb.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Ebomb.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"


Ebomb :: Ebomb()
{
	m_state = UNKNOWN;
	m_radius = 0.0f;
	m_ebombType = EBOMB_TYPE_UNKNOWN;
}

Ebomb :: ~Ebomb()
{
}



void Ebomb :: loadSettings(const ParserSection& ps)
{
	m_radius = FromString<float>(ps.getVal("Radius"));
	Rigidbody::loadSettings(ps);
}