#ifndef _RES_SPAWN_POINT_RENDERER_H_
#define _RES_SPAWN_POINT_RENDERER_H_
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "../GameLogic/Objects/SpawnPoint.h"

#include <vector>

class Graphics;
class Texture;


/**
 * Renders spawn point portals
 *
 */

class SpawnPointRenderer : public EventListener< Spawnpoint_Spawned >,
						public EventListener< Spawnpoint_Despawned >,
						public EventListener< Spawnpoint_SessionStarted >,
						public EventListener< Spawnpoint_SessionEnded >,
						public EventListener< Level_Unload >
					 
{
public:
	SpawnPointRenderer();
	virtual ~SpawnPointRenderer();

	void render(Graphics& g) const;
	void update(const float dt);

	void onEvent(Spawnpoint_Spawned&);
	void onEvent(Spawnpoint_Despawned&);
	void onEvent(Spawnpoint_SessionStarted&);
	void onEvent(Spawnpoint_SessionEnded&);
	void onEvent(Level_Unload&);

private:

	struct SpawnPointInfo_t
	{
		const Spawnpoint * sp;
		float		 elapsedTime;		// The time elapsed after session messages
		bool		 isActive;
		bool		 hasEnded;			// if this is true, then pass 1.0 - elapsedTime/timeInterval to shader
										// if this is false, then pass elapsedTime/timeInterval to shader
										// when it's true & the above diff is < 0, deactivate & reset
		SpawnPointInfo_t(const Spawnpoint * s)
			:sp(s),elapsedTime(0.0f),isActive(false),hasEnded(false){}
	};

	std::vector<SpawnPointInfo_t> m_spawnPoints;
	//const Texture * m_spawnPointTexture;
};


#endif