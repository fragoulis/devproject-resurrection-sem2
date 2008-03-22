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

class HUDRenderer : public IRenderer,
					public EventListener< Player_Spawned >,
					public EventListener< Player_Despawned >
{
public:
	HUDRenderer();
	virtual ~HUDRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);


	// Events
	void onEvent(Player_Spawned&);
	void onEvent(Player_Despawned&);

private:
	Playership* m_playership;
};