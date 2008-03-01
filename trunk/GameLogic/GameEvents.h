#pragma once
#include "../utility/EventManager.h"
#include "EnergyTypes.h"
#include <string>
class Playership;
class Enemyship;
class Terrain;
class ParserSection;

EVENT_WITH_CONST_REFERENCE(Level_Load, ParserSection);
SIMPLE_EVENT(Level_Unload);
EVENT_WITH_POINTER(Terrain_Changed, Terrain);
EVENT_WITH_POINTER(Player_Spawned, Playership);
EVENT_WITH_POINTER(Enemy_Spawned, Enemyship);
EVENT_WITH_POINTER_AND_VALUE(Player_Destroyed, Playership, EnergyType);
EVENT_WITH_POINTER_AND_2VALUES(Player_Drained, Playership, EnergyType, int);
