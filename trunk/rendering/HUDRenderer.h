#pragma once
#include "IRenderer.h"
#include "../GameLogic/GameEvents.h"
#include "../utility/EventManager.h"
#include <vector>

class Texture;
class Playership;

using namespace std;


/**
 * Renders the HUD, including the mouse cursor.
 *
 */
class HUDRenderer : public IRenderer
					//public EventListener< Player_Spawned >,
					//public EventListener< Player_Despawned >,
					//public EventListener< Level_Unload >
{
private:
	enum {
		TEXTURE_RED_BAR,
		NUM_TEXTURES
	};
public:
	HUDRenderer();
	virtual ~HUDRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);


	// Events
	//void onEvent(Player_Spawned&);
	//void onEvent(Player_Despawned&);
	//void onEvent(Level_Unload&);

private:
	//const Playership* m_playership;
	//int m_currentLives;
	//EbombType m_ebombType; 

	vector<Texture *> m_textureList;

};
