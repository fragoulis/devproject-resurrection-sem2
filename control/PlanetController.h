#pragma once
#include "IController.h"
class PlanetRenderer;



class PlanetController : public IController
{
public:
	PlanetController();
	virtual ~PlanetController();

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
	PlanetRenderer* m_renderer;
};
