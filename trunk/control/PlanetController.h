#pragma once
#include "IController.h"
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
class PlanetRenderer;



class PlanetController :
	public IController,
	public Singleton< PlanetController >,
	public EventListener< Level_Complete >
{
public:

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

	virtual void onEvent(Level_Complete&);

	void load();

private:
	PlanetRenderer* m_renderer;


	friend Singleton< PlanetController >;
	PlanetController();
	virtual ~PlanetController();
};
