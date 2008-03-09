//*****************************************************************************
//**
//**   File:               LaserTypes.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include <string>

enum LaserType {
	PLAYER_POSITIVE,
	PLAYER_NEGATIVE,
	LASER_TYPE_COUNT,
	LASER_TYPE_UNKNOWN,
};


//inline std::string StringFromLaserType(LaserType type)
//{
//	switch (type)
//	{
//	case PLAYER_POSITIVE :
//		return "PlayerPositive";
//	case PLAYER_NEGATIVE :
//		return "PlayerNegative";
//	}
//	assert(0);
//}