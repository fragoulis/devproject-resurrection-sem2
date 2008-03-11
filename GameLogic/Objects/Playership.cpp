//*****************************************************************************
//**
//**   File:               Playership.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Playership.h"

Playership :: Playership()
{
	for (int i = 0; i < ENERGY_TYPE_COUNT; i++)
		m_energy[i] = 100;
}

Playership :: ~Playership()
{
}
