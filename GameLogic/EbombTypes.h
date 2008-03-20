//*****************************************************************************
//**
//**   File:               EbombTypes.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************


#pragma once
#include <string>
#include <cassert>

enum EbombType {
	EBOMB_TYPE_RED,
	EBOMB_TYPE_YELLOW,
	EBOMB_TYPE_BLUE,
	EBOMB_TYPE_ORANGE,
	EBOMB_TYPE_GREEN,
	EBOMB_TYPE_PURPLE,
	EBOMB_TYPE_COUNT,
	EBOMB_TYPE_UNKNOWN,
};

inline EbombType EbombTypeFromString(const std::string& s)
{
	if (s == "red") return EBOMB_TYPE_RED;
	if (s == "yellow") return EBOMB_TYPE_YELLOW;
	if (s == "blue") return EBOMB_TYPE_BLUE;
	if (s == "orange") return EBOMB_TYPE_ORANGE;
	if (s == "green") return EBOMB_TYPE_GREEN;
	if (s == "purple") return EBOMB_TYPE_PURPLE;
	assert(0);
	return EBOMB_TYPE_UNKNOWN;
}


inline std::string StringFromEbombType(EbombType type)
{
	if (type == EBOMB_TYPE_RED) return "red";
	if (type == EBOMB_TYPE_YELLOW) return "yellow";
	if (type == EBOMB_TYPE_BLUE) return "blue";
	if (type == EBOMB_TYPE_ORANGE) return "orange";
	if (type == EBOMB_TYPE_GREEN) return "green";
	if (type == EBOMB_TYPE_PURPLE) return "purple";
	if (type == EBOMB_TYPE_UNKNOWN) return "unknown";
	assert(0);
	return "unknown";
}