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
class Ebomb;

// All objects get a spawned event when the object is just created in memory
// similarily all objects get a despawned event when the object is going to be deleted from memory
// Spawned does fire when the level is loaded,
// but Despawned does not fire when the level is unloaded
// ALL game objects will be deleted from memory when the level is unloaded
// Despawn and Level_Unload fire BEFORE any deleting of memory is done

// Some game objects also have a Destroyed event.
// This fires when the object is destroyed in the game
// In some cases, this means the object will be deleted from memory shortly after
// Do NOT assume the object will be deleted from memory right after


EVENT_WITH_2VALUES(Level_Load, const ParserSection*, const std::string&);
SIMPLE_EVENT(Level_Unload);

EVENT_WITH_VALUE(Terrain_Changed, Terrain*);

EVENT_WITH_VALUE(Player_Spawned, Playership*);
EVENT_WITH_VALUE(Player_Despawned, Playership*);
EVENT_WITH_2VALUES(Player_Destroyed, Playership*, EnergyType);
SIMPLE_EVENT(Player_LaserType_Swapped);

// Drained happens when enemy collides with player, but player is not destroyed
// int is amount of energy drained
EVENT_WITH_3VALUES(Player_EnergyDrained, Playership*, EnergyType, int);

// Gained happens when positive laser hits enemy, but not if player is full energy
// int is amount of energy actually gained (may be less than enemy.energyPoints)
EVENT_WITH_3VALUES(Player_EnergyGained, Playership*, EnergyType, int);

// Dispersed happens when negative laser hits enemy, but not if 0 energy
// int is amount of energy actually dispersed (may be less than enemy.energyPoints)
EVENT_WITH_3VALUES(Player_EnergyDispersed, Playership*, EnergyType, int);

// Enemy hit happens when enemy is hit by a laser, but not destroyed
// int is the laser type
EVENT_WITH_2VALUES(Enemy_Hit, Enemyship*, int);
EVENT_WITH_2VALUES(Enemy_Attached_To_Player, Enemyship*, Playership*);

EVENT_WITH_VALUE(Enemy_Spawned, Enemyship*);
EVENT_WITH_VALUE(Enemy_Despawned, Enemyship*);
EVENT_WITH_VALUE(Enemy_Destroyed, Enemyship*);

EVENT_WITH_VALUE(Crater_Spawned, Crater*);
EVENT_WITH_VALUE(Crater_Despawned, Crater*);

EVENT_WITH_VALUE(Spawnpoint_Spawned, Spawnpoint*);
EVENT_WITH_VALUE(Spawnpoint_Despawned, Spawnpoint*);

// spawn points open and close to spawn multiple enemies in quick succession
EVENT_WITH_VALUE(Spawnpoint_SessionStarted, Spawnpoint*);
EVENT_WITH_VALUE(Spawnpoint_SessionEnded, Spawnpoint*);

EVENT_WITH_VALUE(Ebomb_Spawned, Ebomb*);
EVENT_WITH_VALUE(Ebomb_Despawned, Ebomb*);

EVENT_WITH_VALUE(Laser_Spawned, Laser*);
EVENT_WITH_VALUE(Laser_Despawned, Laser*);
