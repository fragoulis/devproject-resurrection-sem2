//*****************************************************************************
//**
//**   File:               SoundEngine.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************

#include "SoundEngine.h"
#include "Sound.h"
#include "SoundObject.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include <iostream>
using namespace std;

#define LOAD_SOUND( root, id ) \
{ \
    const string& sLaserFile = general.getVal(id); \
    Sound *laser = new Sound; \
    if( !laser->load( root + sLaserFile ) ) \
        cerr << "Failed to load sound: " << id << endl; \
    m_sounds.insert( make_pair( id, laser ) ); \
}

SoundEngine :: SoundEngine():
m_listener(0)
{
	EventManager::instance().registerEventListener< Level_Load >(this);
    EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_EnergyDrained >(this);
    EventManager::instance().registerEventListener< Laser_Spawned >(this);
    EventManager::instance().registerEventListener< Enemy_Destroyed >(this);
}

SoundEngine :: ~SoundEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

void SoundEngine :: onApplicationLoad(const ParserSection& ps)
{
    const string& sRootDir = ps.getSection("Sound:Root")->getVal("Dir");
    
    // Read the number of sounds
    const ParserSection& general = *(ps.getSection("Sound:General"));

    // Loads sounds 
    LOAD_SOUND( sRootDir, "Laser_Fired" );
    LOAD_SOUND( sRootDir, "Ambient" );
}

void SoundEngine :: onApplicationUnload()
{
    for( SoundListIter i = m_sounds.begin();
         i != m_sounds.end(); 
         ++i )
    {
        delete i->second;
        i->second = 0;
    }

    for( SoundObjectIter i = m_soundObjects.begin();
         i != m_soundObjects.end(); 
         ++i )
    {
        delete i->second;
        i->second = 0;
    }
}

void SoundEngine :: onEvent(Level_Load& ll)
{
	const ParserSection* ps = ll.getValue1();

	// TODO: make sure all sounds required for this level are loaded
	// If not, load them now.    
}

void SoundEngine :: onEvent(Player_Spawned& pd)
{
	m_listener = pd.getValue();

    // Assign two sound sources to playership
    // one for the lasers and one for the jet engines

    _addSoundObject( m_listener, "Laser_Fired" );
    _addSoundObject( m_listener, "Ambient" );

    m_soundObjects["Ambient"]->play();
}


void SoundEngine :: onEvent(Player_Destroyed& pd)
{
    m_listener = 0;

	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();

	// TODO: play sound effect for playership explosion
}

void SoundEngine :: onEvent(Enemy_Destroyed& pd)
{
}

void SoundEngine :: onEvent(Player_EnergyDrained& pd)
{
	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();
	int amount = pd.getValue3();
	// TODO: play sound effect for energy draining from playership
}

void SoundEngine :: onEvent(Laser_Spawned& pd)
{
    Laser *laser = pd.getValue();

    m_soundObjects["Laser_Fired"]->play();
}

void SoundEngine::update()
{
    _updateListener();

    SoundObjectIter i = m_soundObjects.begin();
    for(; i != m_soundObjects.end(); ++i )
        i->second->update();
}

void SoundEngine::_updateListener()
{
    if( !m_listener ) return;

    float orientation[] = 
    {
        m_listener->getThrusterDirection().getX(),
        m_listener->getThrusterDirection().getY(), 
        m_listener->getThrusterDirection().getZ(),
        0.0f, 1.0f, 0.0f
    };

    alListenerfv(AL_POSITION,    m_listener->getPosition().cfp() );
    alListenerfv(AL_VELOCITY,    m_listener->getVelocity().cfp() );
    alListenerfv(AL_ORIENTATION, orientation );
}

void SoundEngine::_addSoundObject( WorldObject *obj, const sound_id_t &id )
{
    SoundObject *so = new SoundObject( obj, m_sounds[id] );
    m_soundObjects.insert( std::make_pair( id, so ) );
}