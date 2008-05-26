#ifndef _RES_HUD_RENDERER_H_
#define _RES_HUD_RENDERER_H_
#include "IRenderer.h"
#include "../GameLogic/GameEvents.h"
#include "../utility/EventManager.h"
#include <vector>

class Texture;
class Playership;


/**
 * Renders the HUD, including the mouse cursor.
 *
 */
class HUDRenderer : public IRenderer, 
					//public EventListener< Player_Spawned >,
					//public EventListener< Player_Despawned >,
					//public EventListener< Level_Unload >
					public EventListener< Ebomb_Created >,
					public EventListener< Ebomb_Uncreated >
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
	void onEvent(Ebomb_Created&);
	void onEvent(Ebomb_Uncreated&);

private:
	//const Playership* m_playership;
	//int m_currentLives;
	//EbombType m_ebombType; 

	std::vector<Texture *> m_textureList;

	bool m_displayEbombMsg;
	float m_startEbombMessageTime;
	float m_messageDisplayTime;
    float m_ebombMgsTimer;

	float m_currentTime;

    void _displayEbombMessage( float transparency, float in_factor, float in_time ) const;
	void _addTexture(const std::string& name);
};


#endif