#pragma once
#include "IController.h"


/**
 * Handles user input during game over screen.
 * Activates the GameOverRenderer on top of the currently active renderers.
 * Does not call any update on GameLogic.
 */
class GameOverController : public IController
{
public:
	GameOverController();
	virtual ~GameOverController();

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
	void _loadMainMenu();
};
