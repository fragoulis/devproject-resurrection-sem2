//*****************************************************************************
//**
//**   File:               SoundEngine.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************

#include "SoundEngine.h"
#include "SoundBuffer.h"
#include "Sound.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../gfxutils/Misc/utils.h"
#include <iostream>

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
    const string& root = ps.getSection("Sound")->getVal("Dir");

    // Read the number of sounds
    const ParserSection *general = ps.getSection("Sound:Files");

    typedef vector<ParserSection::SectionData> SectionList;
    const SectionList section_list = general->getSectionData();
    const int section_list_size = (int)section_list.size();

    // allocate sound buffers
    m_bufferMemoryPool = new SoundBuffer[section_list_size];

    // load sounds from file tou buffers
    for( int i=0; i<section_list_size; i++ ) 
    {
        const ParserSection::SectionData section = section_list[i];
        const string& id   = section._tag;
        const string& file = section._val[0];

        SoundBuffer *buffer = &m_bufferMemoryPool[i];

        if( !buffer->load( root + file ) ) 
        {
            cerr << "Failed to load sound: " << id << endl;
            continue;
        }

        m_buffers.insert( make_pair( id, buffer ) );
    }

    assert(m_buffers.size());
    
    // allocate some sounds
    const int sounds_count = FromString<int>(ps.getSection("Sound")->getVal("AllocationSize"));
    m_sounds.resize(sounds_count);
    
    m_soundMemoryPool = new Sound[sounds_count];
    for( int i=0; i<sounds_count; i++ ) {
        m_sounds[i] = &m_soundMemoryPool[i];
    }
}

void SoundEngine :: onApplicationUnload()
{
    delete[] m_bufferMemoryPool;
    delete[] m_soundMemoryPool;
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

    //_addSoundObject( "PlayerLaser", m_listener, "Laser_Fired" );

}

void SoundEngine :: onEvent(Player_Destroyed& pd)
{
    m_listener = 0;

	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();

    //cerr << "Player destroyed!" << endl;
	// TODO: play sound effect for playership explosion
}

void SoundEngine :: onEvent(Enemy_Destroyed& pd)
{
    //cerr << "Enemy destroyed!" << endl;
    const Enemyship *enemy = pd.getValue();
    const int iType = enemy->getType();
    std::string sType = WorldObjectTypeManager::instance().getNameFromType(iType);
    sType += "_Destroyed";

    _play(sType);
}

void SoundEngine :: onEvent(Player_EnergyDrained& pd)
{
	Playership* ps = pd.getValue1();
	EnergyType type = pd.getValue2();
	int amount = pd.getValue3();
	// TODO: play sound effect for energy draining from playership

    //cerr << "Energy drained!" << endl;
}

void SoundEngine :: onEvent(Laser_Spawned& pd)
{
    _play("Laser_Fired");
}

void SoundEngine::update()
{
    _updateListener();

    Sounds::iterator i = m_sounds.begin();
    for(; i != m_sounds.end(); ++i )
        (*i)->update( m_listener->getPosition() );
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

void SoundEngine::_play( const string &id )
{
    // search though the sound list to find an empty slot
    bool ok = false;
    Sounds::const_iterator i = m_sounds.begin();
    for(; i != m_sounds.end() && !ok; ++i )
    {
        Sound *sound = *i;
        if( !sound->isPlaying() )
        {
            SoundBuffer *buffer = m_buffers[id];
            if( !buffer ) return;

            sound->set( buffer );
            sound->play();
            ok = true;
        }
    }

    assert(ok);
}