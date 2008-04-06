#pragma once
#include "IController.h"
class PauseRenderer;

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
