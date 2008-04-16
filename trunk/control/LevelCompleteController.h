#pragma once
#include "IController.h"

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
