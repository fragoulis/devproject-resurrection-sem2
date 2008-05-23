#ifndef _RES_PAUSE_CONTROLLER_H_
#define _RES_PAUSE_CONTROLLER_H_
#include "IController.h"
class PauseRenderer;




/**
 * Shows the pause menu during gameplay
 * Handles user input for the menu
 */
class PauseController : public IController
{
public:
	PauseController();
	virtual ~PauseController();

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
	PauseRenderer* m_renderer;


	// Callback to loading controller to go back to main menu
	void _loadMainMenu();
};


#endif