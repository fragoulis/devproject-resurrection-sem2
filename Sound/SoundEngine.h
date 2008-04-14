//*****************************************************************************
//**
//**   File:               SoundEngine.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************

#pragma once
#pragma comment (lib, "openal32")
#pragma comment (lib, "alut")
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
#include <map>
#include <vector>
#include <string>
class ParserSection;
class SoundBuffer;
class Sound;

using namespace std;

/**
 * Listens for events and plays sounds when things happen
 * Events it listens to:
 * - When players spawns, it saves its position to the main listener
 * - When laser fires, it plays a sound
 * - When laser hits an enemy, it plays a sound depending on the enemy type
 * - When an enemy is destroyed, it plays a sound
 * - When player is destroyed, it plays a sound
 * - When ebomb is ready
 * - When ebomb is fired
 * - When ebomb hits successfully
 * - When ebomb hits unsuccessfully
 * - When level is done
 * - engine?
 * - ambient
 */
class SoundEngine :
	public Singleton< SoundEngine >,
	public EventListener< Level_Load >,
    public EventListener< Player_Spawned >,
    public EventListener< Player_Destroyed >,
	public EventListener< Player_EnergyDrained >,
	//public EventListener< Enemy_Hit >,
    public EventListener< Enemy_Destroyed >,
    public EventListener< Laser_Spawned >
{
private:
    typedef std::map< const std::string, SoundBuffer*> SoundBuffers;
    typedef std::vector< Sound*> Sounds;

    SoundBuffers m_buffers;
    Sounds m_sounds;
    Playership *m_listener;

    // Used only internaly at assignment and deletion
    Sound *m_soundMemoryPool;
    SoundBuffer *m_bufferMemoryPool;

public:
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void onEvent(Level_Load&);
	void onEvent(Player_Destroyed&);
	void onEvent(Player_EnergyDrained&);
    void onEvent(Player_Spawned&);
    void onEvent(Laser_Spawned&);
    void onEvent(Enemy_Destroyed&);

    void update();

private:
	friend Singleton< SoundEngine >;
	SoundEngine();
	virtual ~SoundEngine();
    void _updateListener();
    void _play( const string &id );
};
