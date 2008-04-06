#include "PlanetController.h"
#include "../rendering/RenderEngine.h"
#include "../rendering/PlanetRenderer.h"
#include "ControllerManager.h"
#include "OSinterface/Input.h"
#include "LoadingController.h"

PlanetController::PlanetController()
{
	m_renderer = dynamic_cast<PlanetRenderer*>(RenderEngine::instance().getRenderer("planet"));
}

PlanetController::~PlanetController()
{
}



void PlanetController :: activate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	//re.deactivateAllRenderers(); // can do this, or not :D
	re.activateRenderer("planet");
}

void PlanetController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateRenderer("planet");
}

void PlanetController :: update(float dt)
{
	// check user input here
	// if something in menu needs to change, update data
	// and ask MenuRenderer to change the graphics.
	// if you want to exit the menu back to the game:
	// ControllerManager::instance().activateController("game")
	// GameController is currently hacked to load the TestLevel when it is activated
	// Later on we shall have to have communication between controllers hmm...


	// if you want the game to run: GameLogic::instance().update(dt)
	// if you want the graphics to update: RenderEngine::instance().update(dt)
	// problem: can't have some renderers update and others not ATM
	// this design sucks!


	Input& input = Input::instance();

	if (input.isKeyDownOrGoingDown(27))
	{
		LoadingController* lc = new LoadingController();

	}
}
