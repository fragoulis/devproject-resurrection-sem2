#include "SoundEngine.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../gfxutils/Misc/utils.h"
#include <iostream>

VOICE SoundEngine::_voices[MAX_VOICES];

SoundEngine :: SoundEngine():
m_listener(0),
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
	ARInit(aramMemArray, MAX_ARAM_BLOCKS);
	ARQInit();

    // initialize AI subsystem
    AIInit(NULL);

    // initialize AX audio system and MIXer application
    AXInit();
    MIXInit();

	// -----------------------------------------------------------
    // Initialize ARAM audio manager (AM)
    // -----------------------------------------------------------

    // get a block from the AR ARAM allocator
    aramUserBase = ARAlloc(AUDIO_BLOCK_SIZE_BYTES);

    // initialize AM with the block
    AMInit(aramUserBase, AUDIO_BLOCK_SIZE_BYTES);

    // retrieve start of zero buffer, as created by AM
    aramZeroBase = AMGetZeroBuffer();

	// -----------------------------------------------------------
    // Load SP data!
    // -----------------------------------------------------------

    // Retrieve sound table 
    sp_table = (SPSoundTable *)AMLoadFile(SPT_FILE, NULL);

    // Load sound effects into ARAM
    aramUserBase = AMPushBuffered(SPD_FILE, (void *)xfer_buffer, XFER_BUFFER_SIZE_BYTES);

    // -----------------------------------------------------------
    // initialize sound table! 
    // -----------------------------------------------------------
    SPInitSoundTable(sp_table, aramUserBase, aramZeroBase);

	for (int i=0; i<MAX_VOICES; i++)
	{
		_voices[i].ax_voice = NULL;
		_voices[i].sp_entry = NULL;
	}

	AXRegisterCallback(ax_demo_callback);

    // initialize profiling for AX
    AXInitProfile(ax_profile, NUM_AX_PROFILE_FRAMES); 
}

// ----------------------------------------------------------------------------
// clear all sounds and buffers and ambient buffer
void SoundEngine :: onApplicationUnload()
{
}

// ----------------------------------------------------------------------------
// save the player ship as the constant listener
// we need its position to update the OpenAL listener
void SoundEngine :: onEvent(Player_Spawned& pd)
{
	//m_listener = pd.getValue();
    //play("Welcome");
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
    //if( m_activeEplosions == m_maxExplosions ) return;
    //m_activeEplosions++;

    //const Enemyship *enemy = pd.getValue();
    //const int iType = enemy->getType();
    //std::string sType = WorldObjectTypeManager::instance().getNameFromType(iType);
    //sType += "_Destroyed";

    //play(sType);
}

// ----------------------------------------------------------------------------
// play laser sound
void SoundEngine :: onEvent(Laser_Spawned& pd)
{
    //play("Laser_Fired");
}

// ----------------------------------------------------------------------------
// play an info sound when we have an ebomb ready
void SoundEngine :: onEvent(Ebomb_Created& ev)
{
    //play("Ebomb_Ready");
}

// ----------------------------------------------------------------------------
// play a sound when an ebomb is fired
void SoundEngine :: onEvent(Ebomb_Spawned& ev)
{
    //play("Ebomb_Deployed01");
    //play("Ebomb_Deployed02");
}

// ----------------------------------------------------------------------------
// play a sound when an ebomb hits the ground
void SoundEngine :: onEvent(Ebomb_Despawned& ev)
{
    //play("Ebomb_Exp01");
    //play("Ebomb_Exp03");
}

// ----------------------------------------------------------------------------
// play a creative sound for the terraforming
void SoundEngine :: onEvent(Life_Restored& ev)
{
    //play("Ebomb_Succeded");
}

// ----------------------------------------------------------------------------
// load level's ambient background sound
void SoundEngine :: onEvent(Level_Load& ev)
{
    //const std::string sAmbient = ev.getValue1()->getSection("Sound")->getVal("ambient");

    //// create sound buffer
    //if( m_bgBuffer ) delete m_bgBuffer;
    //m_bgBuffer = new SoundBuffer;
    //m_bgBuffer->load(sAmbient);

    //// create sound player
    //if( m_bgSound ) delete m_bgSound;
    //m_bgSound = new Sound(m_bgBuffer);
    //m_bgSound->setLoop(true);
    //m_bgSound->play();

    //play("Welcome");
}

// ----------------------------------------------------------------------------
// unload background music from memory
void SoundEngine :: onEvent(Level_Unload& ev)
{
}

// ----------------------------------------------------------------------------
// play sad sound for when we lose
void SoundEngine :: onEvent(Game_Over& ev)
{
    //m_bgSound->stop();
    //play("Player_Lost");
}

// ----------------------------------------------------------------------------
// play a nice victory sound
void SoundEngine :: onEvent(Level_Complete& ev)
{
    //play("Level_Complete");
}

// ----------------------------------------------------------------------------
// updates all sound sources by positioning them
// at the position of the global listener
void SoundEngine::update()
{
    //_updateListener();

    //Sounds::iterator i = m_sounds.begin();
    //for(; i != m_sounds.end(); ++i )
    //    (*i)->update( m_listener->getPosition() );

    //m_bgSound->update(m_listener->getPosition());
    //m_activeEplosions = 0;
}

// ----------------------------------------------------------------------------
// updates the listener by actually putting him at the 
// position of the player
void SoundEngine::_updateListener()
{
    //if( !m_listener ) return;

    //float orientation[] = 
    //{
    //    m_listener->getThrusterDirection().getX(),
    //    m_listener->getThrusterDirection().getY(), 
    //    m_listener->getThrusterDirection().getZ(),
    //    0.0f, 1.0f, 0.0f
    //};

    //alListenerfv(AL_POSITION,    m_listener->getPosition().cfp() );
    //alListenerfv(AL_VELOCITY,    m_listener->getVelocity().cfp() );
    //alListenerfv(AL_ORIENTATION, orientation );
}

// ----------------------------------------------------------------------------
// initializes OpenAL stuff and sound engine's listeners
void SoundEngine::clearSoundPositions()
{
    //Point3 position(0.0f, 0.0f, 0.0f);

    //float orientation[] = 
    //{
    //    position.getX(), position.getY(), position.getZ(),
    //    0.0f, 1.0f, 0.0f
    //};

    //alListenerfv(AL_POSITION,    position.cfp() );
    //alListenerfv(AL_VELOCITY,    position.cfp() );
    //alListenerfv(AL_ORIENTATION, orientation );

    //Sounds::iterator i = m_sounds.begin();
    //for(; i != m_sounds.end(); ++i )
    //    (*i)->update( position );
}

// ----------------------------------------------------------------------------
// plays the sound with the given id
void SoundEngine::play( const string &id, bool repeat )
{
	const int sound = m_sounds[id];
	if(repeat) play_sfx(sound);
	else
		play_looped_sfx(sound);
}

// ----------------------------------------------------------------------------
// clearly stops a sound from playing
// it searches for the sound with the given id
void SoundEngine::stop( unsigned id )
{
    bool old = OSDisableInterrupts();

    for (int i=0; i<MAX_VOICES; i++)
    {
        if (_voices[i].ax_voice)
        {
            AXSetVoiceState(_voices[i].ax_voice, AX_PB_STATE_STOP);
        }
    }

	OSRestoreInterrupts(old);
}

// ----------------------------------------------------------------------------
void SoundEngine::ax_demo_callback(void)
{

    u32 i;

    for (i=0; i<MAX_VOICES; i++)
    {
        if (_voices[i].ax_voice)
        {
            if ( AX_PB_STATE_STOP == ((_voices[i].ax_voice)->pb.state))
            {
                MIXReleaseChannel(_voices[i].ax_voice);
                AXFreeVoice(_voices[i].ax_voice);
                _voices[i].ax_voice = NULL;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void SoundEngine::play_sfx(u32 sfx)
{
    BOOL old = OSDisableInterrupts();
    VOICE *v = get_voice();

    if (v)
    {

        v->ax_voice = AXAcquireVoice(15, ax_drop_voice_callback, 0);
        if (v->ax_voice)
        {

            v->sp_entry = SPGetSoundEntry(sp_table, sfx);

            SPPrepareSound(v->sp_entry, v->ax_voice, (v->sp_entry)->sampleRate);

            MIXInitChannel(v->ax_voice, 0, 0, -960, -960, 64, 127, 0);
            AXSetVoiceState(v->ax_voice, AX_PB_STATE_RUN);

            OSRestoreInterrupts(old);

        }
        else
        {
            OSRestoreInterrupts(old);
            //DEMOWinLogPrintf(DebugWin, "SFX: AX Voice allocation failed.\n");
        }

    }
    else
    {
        OSRestoreInterrupts(old);
        //DEMOWinLogPrintf(DebugWin, "(No free voices in abstraction layer)\n");
    }

} // end play_sfx()

// ----------------------------------------------------------------------------
void SoundEngine::play_looped_sfx(u32 sfx)
{
    BOOL old = OSDisableInterrupts();
    VOICE *v = get_voice();

    if (v)
    {

        v->ax_voice = AXAcquireVoice(15, ax_drop_voice_callback, 0);
        if (v->ax_voice)
        {
            v->sp_entry = SPGetSoundEntry(sp_table, sfx);
			v->ax_voice->pb.addr.loopFlag = AXPBADDR_LOOP_ON;
			v->sp_entry->type = SP_TYPE_PCM16_LOOPED;
			v->sp_entry->loopAddr = v->sp_entry->currentAddr;
			v->sp_entry->loopEndAddr = v->sp_entry->endAddr;

            SPPrepareSound(v->sp_entry, v->ax_voice, (v->sp_entry)->sampleRate);

			MIXInitChannel(v->ax_voice, 0, 0, -960, -960, 64, 127, 0);
            AXSetVoiceState(v->ax_voice, AX_PB_STATE_RUN);

            OSRestoreInterrupts(old);

        }
        else
        {
            OSRestoreInterrupts(old);
            //DEMOWinLogPrintf(DebugWin, "SFX: AX Voice allocation failed.\n");
        }

    }
    else
    {
        OSRestoreInterrupts(old);
        //DEMOWinLogPrintf(DebugWin, "(No free voices in abstraction layer)\n");
    }

} // end play_looped_sfx()

// ----------------------------------------------------------------------------
VOICE *SoundEngine::get_voice()
{

    u32 i;

        i=0;
        while (i < MAX_VOICES)
        {

            if (NULL == _voices[i].ax_voice)
            {
                return(&_voices[i]);
            }
            i++;
        }

        return(NULL);

}  // end get_demo_voice()

// ----------------------------------------------------------------------------
void SoundEngine::ax_drop_voice_callback(void *p)
{
    u32 i;

    // search for abstracted voice associated with low-level AX voice.
    for (i=0; i<MAX_VOICES; i++)
    {
        // found it! 
        if  ( (AXVPB *)(p) == _voices[i].ax_voice)
        {
            // release mixer channel, delete reference to AX voice (and SP entry, just for neatness)
            MIXReleaseChannel(_voices[i].ax_voice);
            _voices[i].ax_voice = NULL;
            _voices[i].sp_entry = NULL;
            break;
        }
    }

    // freak out if the voice doesn't exist in our voice abstraction list
    ASSERTMSG(i != MAX_VOICES, "AXVoiceCallback: unknown voice reference!\n");
}