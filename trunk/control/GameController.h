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
#include "../utility/Singleton.h"
#include "LevelCompleteController.h"
#include "PauseController.h"
#include "GameOverController.h"
#include <string>


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
	public Singleton<GameController>,
	public EventListener<Level_Complete>,
	public EventListener<Game_Over>
{
public:

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

	virtual void onEvent(Level_Complete&);
	virtual void onEvent(Game_Over&);

	void loadLevel(const std::string& levelName);
	void continueGame();

private:

	// Sort of subcontrollers
	PauseController m_pauseController;
	GameOverController m_gameOverController;
	LevelCompleteController m_levelCompleteController;



	friend Singleton< GameController >;
	GameController();
	virtual ~GameController();
};
