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
	ENERGY_TYPE_RED,
	ENERGY_TYPE_YELLOW,
	ENERGY_TYPE_BLUE,
	ENERGY_TYPE_COUNT,
	ENERGY_TYPE_UNKNOWN,
};

inline EnergyType EnergyTypeFromString(const std::string& s)
{
	if (s == "red") return ENERGY_TYPE_RED;
	if (s == "yellow") return ENERGY_TYPE_YELLOW;
	if (s == "blue") return ENERGY_TYPE_BLUE;
	return ENERGY_TYPE_UNKNOWN;
}