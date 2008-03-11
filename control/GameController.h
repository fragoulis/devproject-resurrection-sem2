//*****************************************************************************
//**
//**   File:               GameController.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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

private:

	void _fireLaser(int type);
};
