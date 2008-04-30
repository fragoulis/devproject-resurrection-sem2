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
    public EventListener< Level_Complete >
{
private:
    typedef std::map< const std::string, SoundBuffer*> SoundBuffers;
    typedef std::vector< Sound*> Sounds;

    SoundBuffers m_buffers;
    Sounds m_sounds;
    Playership *m_listener;

    // separate pointers for large size background music
    SoundBuffer *m_bgBuffer;
    Sound *m_bgSound;

    // Used only internaly at assignment and deletion
    Sound *m_soundMemoryPool;
    SoundBuffer *m_bufferMemoryPool;

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

    void update();
    void clearSoundPositions();
    unsigned play( const string &id, bool repeat = false );
    bool stop( unsigned id );

private:
	friend Singleton< SoundEngine >;
	SoundEngine();
	~SoundEngine();
    void _updateListener();
};
