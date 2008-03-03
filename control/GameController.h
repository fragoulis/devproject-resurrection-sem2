#pragma once
#include "IController.h"
#include "../GameLogic/LaserTypes.h"

class GameController : public IController
{
public:
	GameController();
	virtual ~GameController();


	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:

	void _fireLaser(LaserType type);
};
