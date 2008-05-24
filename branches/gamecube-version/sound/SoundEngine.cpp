#include "SoundEngine.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Lasers/Laser.h"
#include "../GameLogic/EnergyTypes.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../gfxutils/Misc/utils.h"
#include <iostream>

VOICE SoundEngine::m_voices[MAX_VOICES];

SoundEngine :: SoundEngine():
m_listener(0),
m_maxExplosions(0),
m_activeEplosions(0)
{
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
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
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
	cout << "Initializing sound engine ... " << endl;

	ARInit(m_aramMemArray, MAX_ARAM_BLOCKS);
	//cout << "ARInit returned: " << ret << endl;

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
    m_aramUserBase = ARAlloc(AUDIO_BLOCK_SIZE_BYTES);
	//cout << "ARAlloc returned: " << m_aramUserBase << endl;

    // initialize AM with the block
    AMInit(m_aramUserBase, AUDIO_BLOCK_SIZE_BYTES);

    // retrieve start of zero buffer, as created by AM
    m_aramZeroBase = AMGetZeroBuffer();
	//cout << "AMGetZeroBuffer returned: " << m_aramZeroBase << endl;

	// -----------------------------------------------------------
    // Load SP data!
    // -----------------------------------------------------------

    // Retrieve sound table 
	u32 len = 0;
    m_sp_table = (SPSoundTable *)AMLoadFile(SPT_FILE, &len);
	//cout << "AMLoadFile returned length: " << len << endl;

    // Load sound effects into ARAM
    m_aramUserBase = AMPushBuffered(SPD_FILE, (void *)m_xfer_buffer, XFER_BUFFER_SIZE_BYTES);
	//cout << "AMPushBuffered returned: " << m_aramUserBase << endl;

    // -----------------------------------------------------------
    // initialize sound table! 
    // -----------------------------------------------------------
    SPInitSoundTable(m_sp_table, m_aramUserBase, m_aramZeroBase);

	for (int i=0; i<MAX_VOICES; i++)
	{
		m_voices[i].ax_voice = NULL;
		m_voices[i].sp_entry = NULL;
	}

	AXRegisterCallback(ax_demo_callback);

    // initialize profiling for AX
    AXInitProfile(ax_profile, NUM_AX_PROFILE_FRAMES); 

	_matchSoundIdToString();

	cout << "Sound engine initialized!" << endl;
}

// ----------------------------------------------------------------------------
// clear all sounds and buffers and ambient buffer
void SoundEngine :: onApplicationUnload()
{
}

// ----------------------------------------------------------------------------
void SoundEngine::_matchSoundIdToString()
{
#define PUSH_PAIR(key, value) m_sounds.insert( make_pair(key, value) );

	PUSH_PAIR("Laser",			SFX_LASER);	
	PUSH_PAIR("IFaceBgk",		SFX_IFACE_BKG);
	PUSH_PAIR("IFaceClick",		SFX_IFACE_CLICK);
	PUSH_PAIR("EbombDeployed01",SFX_EBOMB_HOLLOW);
	PUSH_PAIR("EbombDeployed02",SFX_EBOMB_BEEP);
	PUSH_PAIR("EbombReady",		SFX_EBOMB_READY);
	PUSH_PAIR("EbombSucceded",	SFX_EBOMB_SUCCEDED);
	PUSH_PAIR("EbombExp01",		SFX_EBOMB_EXP);
	PUSH_PAIR("EbombExp02",		SFX_FIREBALL);
	PUSH_PAIR("PlayerLost",		SFX_PLAYER_LOST);
	PUSH_PAIR("LevelComplete",	SFX_LEVEL_COMPLETE);
	PUSH_PAIR("Welcome",		SFX_WELCOME);
	PUSH_PAIR("Dot",			SFX_DOT);
	PUSH_PAIR("Player_Destroyed",		    SFX_EXP01);
	PUSH_PAIR("EnemyFighter_Destroyed",		SFX_EXP01);
	PUSH_PAIR("EnemyInterceptor_Destroyed", SFX_EXP02);
	PUSH_PAIR("EnemyDisrupter_Destroyed",	SFX_EXP01);
	PUSH_PAIR("EnemyCarrier_Destroyed",		SFX_EXP02);

#undef PUSH_PAIR
}

// ----------------------------------------------------------------------------
void SoundEngine :: onEvent(Player_Destroyed& pd)
{
	play("Player_Destroyed");
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
    play("Laser");
}

// ----------------------------------------------------------------------------
// play an info sound when we have an ebomb ready
void SoundEngine :: onEvent(Ebomb_Created& ev)
{
    play("EbombReady");
}

// ----------------------------------------------------------------------------
// play a sound when an ebomb is fired
void SoundEngine :: onEvent(Ebomb_Spawned& ev)
{
    play("EbombDeployed01");
    play("EbombDeployed02");
}

// ----------------------------------------------------------------------------
// play a sound when an ebomb hits the ground
void SoundEngine :: onEvent(Ebomb_Despawned& ev)
{
    play("EbombExp01");
    play("EbombExp02");
}

// ----------------------------------------------------------------------------
// play a creative sound for the terraforming
void SoundEngine :: onEvent(Life_Restored& ev)
{
    play("EbombSucceded");
}

// ----------------------------------------------------------------------------
// load level's ambient background sound
void SoundEngine :: onEvent(Level_Load& ev)
{
	//play("Dot", true);
    play("Welcome");
}

// ----------------------------------------------------------------------------
// unload background music from memory
void SoundEngine :: onEvent(Level_Unload& ev)
{
	//stop("Dot");
}

// ----------------------------------------------------------------------------
// play sad sound for when we lose
void SoundEngine :: onEvent(Game_Over& ev)
{
    play("PlayerLost");
}

// ----------------------------------------------------------------------------
// play a nice victory sound
void SoundEngine :: onEvent(Level_Complete& ev)
{
    play("LevelComplete");
}

// ----------------------------------------------------------------------------
// plays the sound with the given id
void SoundEngine::play( const string &id, bool repeat )
{
	const int sound = m_sounds[id];
	std::cout << "Trying to play sound[\""<< id << "\"]=" << sound << std::endl;
	if(repeat) play_looped_sfx(sound);
	else
		play_sfx(sound);
}

// ----------------------------------------------------------------------------
// clearly stops a sound from playing
// it searches for the sound with the given id
void SoundEngine::stop( const string &id )
{
	stop_sfx(m_sounds[id]);
}

void SoundEngine::stop_sfx( unsigned id )
{
    bool old = OSDisableInterrupts();

    for (int i=0; i<MAX_VOICES; i++)
    {
        if (m_voices[i].ax_voice)
        {
            AXSetVoiceState(m_voices[i].ax_voice, AX_PB_STATE_STOP);
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
        if (m_voices[i].ax_voice)
        {
            if ( AX_PB_STATE_STOP == ((m_voices[i].ax_voice)->pb.state))
            {
                MIXReleaseChannel(m_voices[i].ax_voice);
                AXFreeVoice(m_voices[i].ax_voice);
                m_voices[i].ax_voice = NULL;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void SoundEngine::play_sfx(u32 sfx)
{
    BOOL old = OSDisableInterrupts();
	//cout << "OSDisableInterrupts() returned: " << old << endl;
    
	VOICE *v = get_voice();

    if (v)
    {
		//cout << "Found an available voice" << endl;

        v->ax_voice = AXAcquireVoice(15, ax_drop_voice_callback, 0);
        if (v->ax_voice)
        {
			//cout << "Acquired the voice" << endl;

            v->sp_entry = SPGetSoundEntry(m_sp_table, sfx);

            SPPrepareSound(v->sp_entry, v->ax_voice, (v->sp_entry)->sampleRate);

            MIXInitChannel(v->ax_voice, 0, 0, -960, -960, 64, 127, 0);
            AXSetVoiceState(v->ax_voice, AX_PB_STATE_RUN);

            OSRestoreInterrupts(old);
        }
        else
        {
			std::cout << "Couldnt acquire the available voice" << std::endl;
            OSRestoreInterrupts(old);
        }
    }
    else
    {
		cout << "Couldn't find an available voice" << endl;
        OSRestoreInterrupts(old);
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
            v->sp_entry = SPGetSoundEntry(m_sp_table, sfx);
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
    u32 i = 0;
    while (i < MAX_VOICES)
    {
        if (NULL == m_voices[i].ax_voice)
        {
            return(&m_voices[i]);
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
        if  ( (AXVPB *)(p) == m_voices[i].ax_voice)
        {
            // release mixer channel, delete reference to AX voice (and SP entry, just for neatness)
            MIXReleaseChannel(m_voices[i].ax_voice);
            m_voices[i].ax_voice = NULL;
            m_voices[i].sp_entry = NULL;
            break;
        }
    }

    // freak out if the voice doesn't exist in our voice abstraction list
    ASSERTMSG(i != MAX_VOICES, "AXVoiceCallback: unknown voice reference!\n");
}