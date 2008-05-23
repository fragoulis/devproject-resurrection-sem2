#ifndef _RES_LEVEL_COMPLETE_CONTROLLER_H_
#define _RES_LEVEL_COMPLETE_CONTROLLER_H_
#include "IController.h"

/**
 * Handles user input during level complete screen
 * Activates the LevelCompleteRenderer on top of the currently active renderers.
 * Does not call any update on GameLogic.
 */
class LevelCompleteController : public IController
{
public:
	LevelCompleteController();
	virtual ~LevelCompleteController();

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
	void _loadPlanetSelectionScreen();
};


#endif