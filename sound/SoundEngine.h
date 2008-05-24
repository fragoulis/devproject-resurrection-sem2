#ifndef _RES_SOUNDENGINE_H_
#define _RES_SOUNDENGINE_H_

#include <dolphin.h>
#include <dolphin/mix.h>
#include <dolphin/sp.h>
#include <dolphin/am.h>
#include "sounds.h"
#include "../utility/Singleton.h"
#include "../GameLogic/GameEvents.h"
#include "../control/OSinterface/Input.h"
#include <map>
#include <string>
class ParserSection;

using namespace std;

//SP data 
#define SPT_FILE "resources/sounds/sounds.spt"
#define SPD_FILE "resources/sounds/sounds.spd"

//AX Profiling
#define NUM_AX_PROFILE_FRAMES 8 // store up to 8 frames, just to be safe

//ARAM initialization
#define MAX_ARAM_BLOCKS  3 // Use AR allocator to divide ARAM into 3 blocks
#define AUDIO_BLOCK_SIZE_BYTES (12*1024*1024) // Give a whopping 12MB of ARAM to audio!
#define XFER_BUFFER_SIZE_BYTES (16*1024) // transfer buffer for ARAM audio manager (AM)

//Application-layer voice abstraction
typedef struct 
{
    AXVPB *ax_voice;
    SPSoundEntry *sp_entry;
} VOICE;

#define MAX_VOICES  64

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
    public EventListener< Player_Spawned >,
    public EventListener< Player_Respawned >,
    public EventListener< Enemy_Destroyed >,
    public EventListener< Laser_Spawned >,
    public EventListener< Ebomb_Spawned >,
    public EventListener< Ebomb_Despawned >,
    public EventListener< Ebomb_Created >,
    public EventListener< Life_Restored >,
    public EventListener< Level_Load >,
    public EventListener< Level_Unload >,
    public EventListener< Game_Over >,
    public EventListener< Level_Complete >,
	public EventListener< Button_GoingDown >
{
private:
    Playership *m_listener;

    // maximum enemy explosions at any time
    int m_maxExplosions;
    int m_activeEplosions;

	typedef map<string,u32> Sounds;
	Sounds m_sounds;

	static VOICE m_voices[MAX_VOICES];
	SPSoundTable *m_sp_table;
	AXPROFILE ax_profile[NUM_AX_PROFILE_FRAMES]; 
	u32 m_aramZeroBase, m_aramUserBase, m_aramMemArray[MAX_ARAM_BLOCKS];
	u8 m_xfer_buffer[XFER_BUFFER_SIZE_BYTES] ATTRIBUTE_ALIGN(32);

public:
	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

    void onEvent(Player_Spawned&);
    void onEvent(Player_Respawned&);
    void onEvent(Enemy_Destroyed&);
    void onEvent(Laser_Spawned&);
    void onEvent(Ebomb_Spawned&);
    void onEvent(Ebomb_Despawned&);
    void onEvent(Ebomb_Created&);
    void onEvent(Life_Restored&);
    void onEvent(Level_Load&);
    void onEvent(Level_Unload&);
    void onEvent(Level_Complete&);
    void onEvent(Game_Over&);
	void onEvent(Button_GoingDown&);

    void update();
    void clearSoundPositions();
    void play( const string &id, bool repeat = false );
    void stop( unsigned id );

private:
	friend Singleton< SoundEngine >;
	SoundEngine();
	~SoundEngine();
    void _updateListener();

	VOICE *get_voice();
	void play_sfx(u32 sfx);
	void play_looped_sfx(u32 sfx);
	static void ax_demo_callback(void);
	static void ax_drop_voice_callback(void *p);

	void _matchSoundIdToString();
};

#endif // _RES_SOUNDENGINE_H_