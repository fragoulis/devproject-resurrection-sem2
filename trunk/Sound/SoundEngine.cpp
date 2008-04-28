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
    EventManager::instance().registerEventListener< Player_Spawned >(this);
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

    //assert(m_buffers.size());
    
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
    m_buffers.clear();
    m_sounds.clear();
    delete[] m_bufferMemoryPool;
    delete[] m_soundMemoryPool;
}

void SoundEngine :: onEvent(Player_Spawned& pd)
{
	m_listener = pd.getValue();
}

void SoundEngine :: onEvent(Enemy_Destroyed& pd)
{
    //cerr << "Enemy destroyed!" << endl;
    const Enemyship *enemy = pd.getValue();
    const int iType = enemy->getType();
    std::string sType = WorldObjectTypeManager::instance().getNameFromType(iType);
    sType += "_Destroyed";

    play(sType);
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
    play("Laser_Fired");
}

// updates all sound sources by positioning them
// at the position of the global listener
void SoundEngine::update()
{
    _updateListener();

    Sounds::iterator i = m_sounds.begin();
    for(; i != m_sounds.end(); ++i )
        (*i)->update( m_listener->getPosition() );
}

// updates the listener by actually putting him at the 
// position of the player
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

// initializes OpenAL stuff and sound engine's listeners
void SoundEngine::clearSoundPositions()
{
    Point3 position(0.0f, 0.0f, 0.0f);

    float orientation[] = 
    {
        position.getX(), position.getY(), position.getZ(),
        0.0f, 1.0f, 0.0f
    };

    alListenerfv(AL_POSITION,    position.cfp() );
    alListenerfv(AL_VELOCITY,    position.cfp() );
    alListenerfv(AL_ORIENTATION, orientation );

    Sounds::iterator i = m_sounds.begin();
    for(; i != m_sounds.end(); ++i )
        (*i)->update( position );
}

// plays the sound with the given id
unsigned SoundEngine::play( const string &id, bool repeat )
{
    // search though the sound list to find an empty slot
    unsigned source = 0;
    Sounds::const_iterator i = m_sounds.begin();
    for(; i != m_sounds.end() && !source; ++i )
    {
        Sound *sound = *i;
        if( !sound->isPlaying() )
        {
            SoundBuffer *buffer = m_buffers[id];
            if( !buffer ) return 0;

            sound->set( buffer );
            sound->setLoop( repeat );
            sound->play();
            source = sound->getSource();
        }
    }

    assert(source);
    return source;
}

// clearly stops a sound from playing
// it searches for the sound with the given id
bool SoundEngine::stop( unsigned id )
{
    Sounds::const_iterator i = m_sounds.begin();
    for(; i != m_sounds.end(); ++i )
    {
        Sound *sound = *i;
        if( sound->isPlaying() && sound->getSource() == id )
        {
            sound->stop();
            return 1;
        }
    }

    return 0;
}