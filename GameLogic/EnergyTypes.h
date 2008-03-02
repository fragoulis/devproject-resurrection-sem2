#pragma once
#include <string>

enum EnergyType { RED, YELLOW, BLUE, ENERGY_TYPE_COUNT, UNKNOWN };

inline EnergyType EnergyTypeFromString(const std::string& s)
{
	if (s == "red") return RED;
	if (s == "yellow") return YELLOW;
	if (s == "blue") return BLUE;
	return UNKNOWN;
}