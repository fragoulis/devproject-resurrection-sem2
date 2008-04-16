//*****************************************************************************
//**
//**   File:               GameController.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-11 23:43:03 +0000 (Tue, 11 Mar 2008) $
//**   Last Revision:      $Revision: 144 $
//**
//*****************************************************************************


#pragma once
#include "IController.h"
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"


/**
 * Controls gameplay in a level only.
 * update checks input for movement and firing lasers.
 * update also calls:
 *   AIEngine::update 
 *   PhysicsEngine::update
 *   GameLogic::update
 *   RenderEngine::update
 *
 * A different controller (PauseController) can decide not to call these functions.
 * That would effectively pause the game.
 */
class GameController :
	public IController,
	public EventListener<Level_Complete>,
	public EventListener<Game_Over>
{
public:
	GameController();
	virtual ~GameController();

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

	virtual void onEvent(Level_Complete&);
	virtual void onEvent(Game_Over&);
};
