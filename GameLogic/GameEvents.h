//*****************************************************************************
//**
//**   File:               GameEvents.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../utility/EventManager.h"
#include "EnergyTypes.h"
#include <string>
class Playership;
class Enemyship;
class Terrain;
class ParserSection;
class Crater;
class Spawnpoint;
class Laser;

EVENT_WITH_2VALUES(Level_Load, const ParserSection*, const std::string&);
SIMPLE_EVENT(Level_Unload);
EVENT_WITH_POINTER(Terrain_Changed, Terrain);
EVENT_WITH_POINTER(Player_Spawned, Playership);
EVENT_WITH_POINTER(Enemy_Spawned, Enemyship);
EVENT_WITH_POINTER(Enemy_Destroyed, Enemyship);
EVENT_WITH_POINTER_AND_VALUE(Player_Destroyed, Playership, EnergyType);
EVENT_WITH_POINTER_AND_2VALUES(Player_Drained, Playership, EnergyType, int); // int is amount of energy drained
EVENT_WITH_POINTER(Crater_Spawned, Crater);
EVENT_WITH_POINTER(Spawnpoint_Spawned, Spawnpoint);
EVENT_WITH_POINTER_AND_2VALUES(Enemy_Hit, Enemyship, EnergyType, int);
EVENT_WITH_2POINTERS(Enemy_Attached_To_Player, Enemyship, Playership);
SIMPLE_EVENT(Player_LaserType_Swapped);
EVENT_WITH_POINTER(Laser_Spawned, Laser);
EVENT_WITH_POINTER(Laser_Despawned, Laser);
