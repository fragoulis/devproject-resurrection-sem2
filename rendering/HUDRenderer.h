//*****************************************************************************
//**
//**   File:               HUDRenderer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"
#include "../GameLogic/GameEvents.h"
#include "../utility/EventManager.h"
#include <vector>

class Texture;
class Playership;

using namespace std;

class HUDRenderer : public IRenderer,
					public EventListener< Player_Spawned >,
					public EventListener< Player_Despawned >
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
	void onEvent(Player_Spawned&);
	void onEvent(Player_Despawned&);

private:
	const Playership* m_playership;
	int m_currentLives;
	EbombType m_ebombType; 

	vector<Texture *> m_textureList;

};
