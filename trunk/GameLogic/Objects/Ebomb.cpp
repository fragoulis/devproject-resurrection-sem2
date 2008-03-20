//*****************************************************************************
//**
//**   File:               Ebomb.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
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