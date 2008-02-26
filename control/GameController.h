#pragma once
#include "IController.h"

class GameController : public IController
{
public:
	GameController();
	virtual ~GameController();


	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
};
