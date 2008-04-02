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
#include "../math/Point3.h"
#include <map>
#include <vector>
#include <string>
class ParserSection;
class Sound;
class SoundSource;

/**
 * Listens for events and plays sounds when things happen
 * Events it listens to:
 * - When players spawns, it saves its position to the main listener
 * - When laser fires, it plays a sound
 * - When laser hits an enemy, it plays a sound
 * - When an enemy is destroyed, it plays a sound
 * - When player is destroyed, it plays a sound
 * - When ebomb is ready
 * - When ebomb is fired
 * - When ebomb hits successfully
 * - When ebomb hits unsuccessfully
 * - When level is done
 */
class SoundEngine :
	public Singleton< SoundEngine >,
	public EventListener< Level_Load >,
    public EventListener< Player_Spawned >,
    public EventListener< Player_Destroyed >,
	public EventListener< Player_EnergyDrained >,
	//public EventListener< Enemy_Hit >,
    //public EventListener< Enemy_Destroyed >,
    public EventListener< Laser_Spawned >
{
private:
    typedef std::string sound_id_t;
    typedef std::map< const sound_id_t, Sound*> SoundList;
    typedef std::vector<SoundSource*> SoundSourceList;

    SoundList m_soundlist;
    SoundSourceList m_soundsourcelist;

    Point3 m_listener;

public:
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	void onEvent(Level_Load&);
	void onEvent(Player_Destroyed&);
	void onEvent(Player_EnergyDrained&);
    void onEvent(Player_Spawned&);
    void onEvent(Laser_Spawned&);

private:
	friend Singleton< SoundEngine >;
	SoundEngine();
	virtual ~SoundEngine();

    void _setListener( const Point3& );
    void _play( const sound_id_t&, const Point3& );
    SoundSource* _addSource( const Point3& );
};
