//*****************************************************************************
//**
//**   File:               GameEvents.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


/**
 * This file defines all the events fired by the gameplay.
 * This excludes events fired from physics or AI.
 * The engine relies heavily on these events,
 * so they are commented heavily.
 * Macros from EventManager.h are used to actually create the events.
 */

#pragma once
#include "../utility/EventManager.h"
#include "EnergyTypes.h"
#include "EbombTypes.h"
#include <string>
class Playership;
class Enemyship;
class Terrain;
class ParserSection;
class Crater;
class Spawnpoint;
class Laser;
class Ebomb;
class Point3;
class EnemyCarrier;

// In general, all events fire AFTER the game state has been updated to reflect the change
// The exception is Despawned and Level_Unload.

// Spawned and Despawned are technical events, unrelated to gameplay.
// All objects get a spawned event when the object is created in memory
// When the spawned event fires, the object has been fully initialized by GameLogic
// This means that Enemyships will have the right position and energy type,
// Lasers will have the right direction, type and velocity, etc.
// Similarily all objects get a despawned event when the object is going to be deleted from memory
// Despawned fires before the object is actually deleted. The object data is still valid for
// the game, but the "to be deleted" flag is set.
// 
// A spawned event fires for every object created when the level is loaded,
// but Despawned does not fire when the level is unloaded
// ALL game objects will be deleted from memory when the level is unloaded
// Despawn and Level_Unload fire BEFORE any deleting of memory is done

// Some game objects also have a Created or Destroyed event.
// These are unrelated to Spawned and Despawned.
// Created fires for example when an Ebomb is created in the cargo bay of the playership
// An actual Ebomb object does not even exist yet.
// Destroyed fires when the object is destroyed in the game, like a big explosion for enemies.
// The playership will fire a destroyed event when the energy reaches 0,
// but the actual playership object will remain in memory and will be reused.


// Fires right before GameLogic starts loading the new level
// The first value is the root section of the level data file (TestLevel.txt)
// The second value is the ID of the level
// All loading from file should be done in response to this event
// Right after this event GameLogic will start spawning objects and fire events:
// Terrain_Changed
// Player_Spawned
// Crater_Spawned for each crater
// Spawnpoint_Spawned for each spawn point
EVENT_WITH_2VALUES(Level_Load, const ParserSection*, const std::string&);

// Level unload fires right before GameLogic starts deleting memory for the entire level
// Despawn events will NOT fire when a level is unloaded
SIMPLE_EVENT(Level_Unload);

// Signals a switch of terrain
// Only 1 terrain object is active at a time
// When this event fires, the previous terrain is despawned,
// and a new terrain is spawned
// Currently fires right after Level_Load
// The parameter is the new terrain object
EVENT_WITH_VALUE(Terrain_Changed, Terrain*);

// Signal the creation of a Playership object in memory
// The object is fully initialized and in a valid game state
// Currently fires right after Level_Load
// The parameter is the new Playership object
EVENT_WITH_VALUE(Player_Spawned, Playership*);

// Fires right before deletion of the Playership object
// Currently doesn't fire at all
// The parameter is the deleted Playership object
EVENT_WITH_VALUE(Player_Despawned, Playership*);

// Signals the destruction of the Playership through collision with an enemy
// If the player has not enough energy to survive, this fires
// If the player does have enough energy to survive, a Player_EnergyDrained event fires
// The first parameter is the playership
// The second the energy type that was drained
EVENT_WITH_2VALUES(Player_Destroyed, Playership*, EnergyType);

// Signals  that the player's lasers are swapped
// This happens when a special type of enemy attached to the player's ship
SIMPLE_EVENT(Player_Laser_Swapped);

// Signals energy lost when the player collides with an enemy
// The player is not destroyed. If the player is destroyed (has not enough energy),
// Player_Destroyed is fired, not this event.
// The first parameter is the playership
// The second the type of energy that was drained,
// The third the amount of energy drained (according to design doc 10)
EVENT_WITH_3VALUES(Player_EnergyDrained, Playership*, EnergyType, int);

// Signals energy gain by positive laser hitting an enemy
// Does not fire if the player has reached the total energy capacity and cannot store more energy.
// The first parameter is the playership
// The second the type of energy that was gained
// The third the amount of energy actually gained
// This may be less than the energy stored in the enemyship that was destroyed (capacity reached)
EVENT_WITH_3VALUES(Player_EnergyGained, Playership*, EnergyType, int);

// Signals energy lost when the negative laser hits an enemy
// Does not fire if the player has no energy of that type to lose
// The first 
// The first parameter is the playership
// The second the type of energy that was lost
// The third the amount of energy actually lost
// This may be less than the energy stored in the enemyship that was destroyed
EVENT_WITH_3VALUES(Player_EnergyDispersed, Playership*, EnergyType, int);

// Signals an enemy getting hit by a laser (positive or negative), but not destroyed
// If the enemy is destroyed, this does not fire; Enemy_Destroyed fires instead.
// The first parameter is the enemy ship
// The second the type of laser
EVENT_WITH_2VALUES(Enemy_Hit, Enemyship*, int);

// Signals an enemy attaching to the player's ship
// The first parameter is the enemy ship
// The second the player ship
EVENT_WITH_2VALUES(Enemy_Attached_To_Player, Enemyship*, Playership*);

// Fires when an Enemyship object is created in memory
// The object is fully initialized and ready to go (in valid game state)
// The parameter is the Enemyship object
EVENT_WITH_VALUE(Enemy_Spawned, Enemyship*);

// Fires right before an Enemyship object is deleted from memory
// The object is in valid game state, but the "to be deleted" flag is set
// The parameter is the Enemyship object
EVENT_WITH_VALUE(Enemy_Despawned, Enemyship*);

// Signals the destruction of an enemy ship
// Enemies are destroyed by getting hit by player lasers,
// or by colliding with the player's ship
// Does not fire for enemies that attach to the player ship
// Does not fire when an enemy is hit by a laser, but not destroyed
// The first parameter is the enemy ship object
EVENT_WITH_VALUE(Enemy_Destroyed, Enemyship*);

// Fires when a Crater object is created in memory
// The object is fully initialized and ready to go (in valid game state)
// The parameter is the Crater object
EVENT_WITH_VALUE(Crater_Spawned, Crater*);

// Fires right before a Crater object is deleted from memory
// The object is in valid game state, but the "to be deleted" flag is set
// The parameter is the Crater object
EVENT_WITH_VALUE(Crater_Despawned, Crater*);

// Signals the restoration of life to an area centered around a location
// Fires when a crater is hit by an ebomb of the same type
// The parameter is the crater that got hit
EVENT_WITH_VALUE(Life_Restored, Crater*);

// Fires when a Spawnpoint object is created in memory
// The object is fully initialized and ready to go (in valid game state)
// The parameter is the Spawnpoint object
EVENT_WITH_VALUE(Spawnpoint_Spawned, Spawnpoint*);

// Fires right before a Spawnpoint object is deleted from memory
// The object is in valid game state, but the "to be deleted" flag is set
// The parameter is the Spawnpoint object
EVENT_WITH_VALUE(Spawnpoint_Despawned, Spawnpoint*);

// Signals the start of a spawning session
// Spawn points operate in sessions.
// During a single session, multiple enemies can spawn
// No enemies spawn if the spawnpoint is not currently "in session"
// The parameter is the spawnpoint
EVENT_WITH_VALUE(Spawnpoint_SessionStarted, Spawnpoint*);

// Signals the end of a spawning session
// The parameter is the spawnpoint
EVENT_WITH_VALUE(Spawnpoint_SessionEnded, Spawnpoint*);

// Signals the creation of an Ebomb in the player ship's cargo bay
// No Ebomb object is created in memory
// It fires when enough energy has been gathered to create an Ebomb
// The parameter is the type of ebomb that was created
EVENT_WITH_VALUE(Ebomb_Created, EbombType);

// Signals the uncreation of an ebomb in the player ship's cargo bay
// No Ebomb object is deleted from memory
// It fires when the player has an Ebomb in the cargo bay
// and loses (part of) the energy needed to create the ebomb
// The parameter is the type of ebomb that was uncreated
EVENT_WITH_VALUE(Ebomb_Uncreated, EbombType);

// Signals an ebomb missing
// Fires when an ebomb hits a crater of a different type,
// and when an ebomb hits the terrain (not a crater)
// The parameter is the Ebomb object
EVENT_WITH_VALUE(Ebomb_Missed, Ebomb*);

// Fires when a Ebomb object is created in memory
// The object is fully initialized and ready to go (in valid game state)
// The parameter is the Ebomb object
EVENT_WITH_VALUE(Ebomb_Spawned, Ebomb*);

// Fires right before a Ebomb object is deleted from memory
// The object is in valid game state, but the "to be deleted" flag is set
// The parameter is the Ebomb object
EVENT_WITH_VALUE(Ebomb_Despawned, Ebomb*);

// Fires when a Laser object is created in memory
// The object is fully initialized and ready to go (in valid game state)
// The parameter is the Laser object
EVENT_WITH_VALUE(Laser_Spawned, Laser*);

// Fires right before a Laser object is deleted from memory
// The object is in valid game state, but the "to be deleted" flag is set
// The parameter is the Laser object
EVENT_WITH_VALUE(Laser_Despawned, Laser*);

// Signals player loses
// Fires when player ship is destroyed and no spare lives remain
SIMPLE_EVENT(Game_Over);

// Signals completion of level
// Fires when player has fixed all craters
// String is level name
EVENT_WITH_VALUE(Level_Complete, const std::string&);

// Signals the player ship respawns
// Respawn is unrelated to spawn/despawn
// Fires when the player is destroyed and at least 1 spare life remains
// The parameter is the player ship object
EVENT_WITH_VALUE(Player_Respawned, Playership*);




// Signals the start of a spawning session
// Spawn points operate in sessions.
// During a single session, multiple enemies can spawn
// No enemies spawn if the spawnpoint is not currently "in session"
// The parameter is the spawnpoint
EVENT_WITH_VALUE(Carrier_SessionStarted, EnemyCarrier*);

// Signals the end of a spawning session
// The parameter is the spawnpoint
EVENT_WITH_VALUE(Carrier_SessionEnded, EnemyCarrier*);

// Signals the collision between an interceptor and the player.
// Its an alternative to Enemy_Destroyed. The passed value is
// a pointer the interceptor clamped.
EVENT_WITH_VALUE(Interceptor_Clamped, Enemyship*);