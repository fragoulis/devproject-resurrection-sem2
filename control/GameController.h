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
class GameController : public IController
{
public:
	GameController();
	virtual ~GameController();


	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

};
