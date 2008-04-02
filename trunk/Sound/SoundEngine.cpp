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
#include "SoundSource.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include <iostream>
using namespace std;

SoundEngine :: SoundEngine()
{
	EventManager::instance().registerEventListener< Level_Load >(this);
    EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_EnergyDrained >(this);
    EventManager::instance().registerEventListener< Laser_Spawned >(this);
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
    const string& sLaserFile = general.getVal("Laser_Fired");

    Sound *laser = new Sound;
    if( !laser->load( sRootDir + sLaserFile ) )
        cerr << "Failed to load sound for laser" << endl;

    m_soundlist.insert( make_pair( "Laser_Fired", laser ) );
}

void SoundEngine :: onApplicationUnload()
{
    SoundList::iterator i = m_soundlist.begin();
    for(; i != m_soundlist.end(); ++i )
    {
        delete i->second;
        i->second = 0;
    }

    SoundSourceList::iterator ss = m_soundsourcelist.begin();
    for(; ss != m_soundsourcelist.end(); ++ss )
        delete *ss;
}

void SoundEngine :: onEvent(Level_Load& ll)
{
	const ParserSection* ps = ll.getValue1();

	// TODO: make sure all sounds required for this level are loaded
	// If not, load them now.
}

void SoundEngine :: onEvent(Player_Spawned& pd)
{
	Playership* ps = pd.getValue();
    _setListener( ps->getPosition() );
}


void SoundEngine :: onEvent(Player_Destroyed& pd)
{
	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();

	// TODO: play sound effect for playership explosion
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
    _play( "Laser_Fired", laser->getPosition() );
}

void SoundEngine::_play( const sound_id_t& sound_id, const Point3& at )
{
    // Get sound
    const ALuint buffer = m_soundlist[ sound_id ]->getBuffer();

    SoundSource *source = _addSource( at );
    if( source->init(&buffer) )
        source->play();
}

SoundSource* SoundEngine::_addSource( const Point3& at )
{
    // Search for an inactive sound source, else allocate
    SoundSource *source = 0;
    
    SoundSourceList::iterator i = m_soundsourcelist.begin();
    for(; i != m_soundsourcelist.end(); ++i )
    {
        SoundSource *cur = *i;
        if( !cur->isPlaying() )
        {
            source = cur;
            break;
        }
    }

    if( !source ) 
    {
        source = new SoundSource;
        m_soundsourcelist.push_back(source);
    }
    source->setPosition( at );

    return source;
}

void SoundEngine::_setListener( const Point3 &position )
{
    m_listener = position;
    cerr << "Listener places at " << position << endl;

    alListenerfv(AL_POSITION,    m_listener.cfp() );
    //alListenerfv(AL_VELOCITY,    ListenerVel);
    //alListenerfv(AL_ORIENTATION, ListenerOri);
}