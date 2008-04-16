#pragma once
#include "IController.h"

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
