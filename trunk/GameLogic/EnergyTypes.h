//*****************************************************************************
//**
//**   File:               EnergyTypes.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include <string>

enum EnergyType {
	RED,
	YELLOW,
	BLUE,
	ENERGY_TYPE_COUNT,
	ENERGY_TYPE_UNKNOWN,
};

inline EnergyType EnergyTypeFromString(const std::string& s)
{
	if (s == "red") return RED;
	if (s == "yellow") return YELLOW;
	if (s == "blue") return BLUE;
	return ENERGY_TYPE_UNKNOWN;
}