#pragma once
#include "IController.h"

/**
 * Shows a loading screen, disables all user input
 *
 */
class LoadingController : public IController
{
public:
	LoadingController();
	virtual ~LoadingController();

	virtual void activate();
	virtual void deactivate();
	virtual void update();

private:
};
