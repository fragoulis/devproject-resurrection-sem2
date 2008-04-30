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
m_listener(0),
m_bgBuffer(0),
m_bgSound(0),
m_maxExplosions(0),
m_activeEplosions(0)
{
    EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Respawned >(this);
    EventManager::instance().registerEventListener< Enemy_Destroyed >(this);
    EventManager::instance().registerEventListener< Laser_Spawned >(this);
	EventManager::instance().registerEventListener< Ebomb_Spawned >(this);
    EventManager::instance().registerEventListener< Ebomb_Despawned >(this);
    EventManager::instance().registerEventListener< Ebomb_Created >(this);
    EventManager::instance().registerEventListener< Life_Restored >(this);
    EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);
    EventManager::instance().registerEventListener< Game_Over >(this);
    EventManager::instance().registerEventListener< Level_Complete >(this);
}

SoundEngine :: ~SoundEngine()
{
	// TODO: when we implement a loading screen, remove this
	onApplicationUnload();
}

// ----------------------------------------------------------------------------
// Load most sounds, interface and game sounds, except the ambient background ones
void SoundEngine :: onApplicationLoad(const ParserSection& ps)
{
    const ParserSection *soundSection = ps.getSection("Sound");

    m_maxExplosions = FromString<int>(soundSection->getVal("MaxExplosions"));
    const string& root = soundSection->getVal("Dir");

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
    const int sounds_count = FromString<int>(soundSection->getVal("AllocationSize"));
    m_sounds.resize(sounds_count);
    
    m_soundMemoryPool = new Sound[sounds_count];
    for( int i=0; i<sounds_count; i++ ) {
        m_sounds[i] = &m_soundMemoryPool[i];
    }
}

// ----------------------------------------------------------------------------
// clear all sounds and buffers and ambient buffer
void SoundEngine :: onApplicationUnload()
{
    m_buffers.clear();
    m_sounds.clear();
    delete[] m_bufferMemoryPool;
    delete[] m_soundMemoryPool;
}

// ----------------------------------------------------------------------------
// save the player ship as the constant listener
// we need its position to update the OpenAL listener
void SoundEngine :: onEvent(Player_Spawned& pd)
{
	m_listener = pd.getValue();
}

// ----------------------------------------------------------------------------
// play a sound to let the player know he has been revived
void SoundEngine :: onEvent(Player_Respawned& pd)
{
}

// ----------------------------------------------------------------------------
// play an explosion sound, depending on the enemy type, as loaded
void SoundEngine :: onEvent(Enemy_Destroyed& pd)
{
    if( m_activeEplosions == m_maxExplosions ) return;
    m_activeEplosions++;

    const Enemyship *enemy = pd.getValue();
    const int iType = enemy->getType();
    std::string sType = WorldObjectTypeManager::instance().getNameFromType(iType);
    sType += "_Destroyed";

    play(sType);
}

// ----------------------------------------------------------------------------
// play laser sound
void SoundEngine :: onEvent(Laser_Spawned& pd)
{
    play("Laser_Fired");
}

// ----------------------------------------------------------------------------
// play an info sound when we have an ebomb ready
void SoundEngine :: onEvent(Ebomb_Created& ev)
{
    play("Ebomb_Ready");
}

// ----------------------------------------------------------------------------
// play a sound when an ebomb is fired
void SoundEngine :: onEvent(Ebomb_Spawned& ev)
{
    play("Ebomb_Deployed01");
    play("Ebomb_Deployed02");
}

// ----------------------------------------------------------------------------
// play a sound when an ebomb hits the ground
void SoundEngine :: onEvent(Ebomb_Despawned& ev)
{
    play("Ebomb_Exp01");
    play("Ebomb_Exp03");
}

// ----------------------------------------------------------------------------
// play a creative sound for the terraforming
void SoundEngine :: onEvent(Life_Restored& ev)
{
    play("Ebomb_Succeded");
}

// ----------------------------------------------------------------------------
// load level's ambient background sound
void SoundEngine :: onEvent(Level_Load& ev)
{
    const std::string sAmbient = ev.getValue1()->getSection("Sound")->getVal("ambient");

    // create sound buffer
    if( m_bgBuffer ) delete m_bgBuffer;
    m_bgBuffer = new SoundBuffer;
    m_bgBuffer->load(sAmbient);

    // create sound player
    if( m_bgSound ) delete m_bgSound;
    m_bgSound = new Sound(m_bgBuffer);
    m_bgSound->setLoop(true);
    m_bgSound->play();
}

// ----------------------------------------------------------------------------
// unload background music from memory
void SoundEngine :: onEvent(Level_Unload& ev)
{
    m_bgSound->stop();
    delete m_bgBuffer; m_bgBuffer = 0;
    delete m_bgSound; m_bgSound = 0;
}

// ----------------------------------------------------------------------------
// play sad sound for when we lose
void SoundEngine :: onEvent(Game_Over& ev)
{
    play("Player_Lost");
}

// ----------------------------------------------------------------------------
// play a nice victory sound
void SoundEngine :: onEvent(Level_Complete& ev)
{
    play("Level_Complete");
}

// ----------------------------------------------------------------------------
// updates all sound sources by positioning them
// at the position of the global listener
void SoundEngine::update()
{
    _updateListener();

    Sounds::iterator i = m_sounds.begin();
    for(; i != m_sounds.end(); ++i )
        (*i)->update( m_listener->getPosition() );

    m_activeEplosions = 0;
}

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
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