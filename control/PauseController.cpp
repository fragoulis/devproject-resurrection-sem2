#include "PauseController.h"
#include "../rendering/RenderEngine.h"
#include "../rendering/PauseRenderer.h"
#include "ControllerManager.h"
#include "OSinterface/Input.h"
#include "LoadingController.h"
#include "GameController.h"
#include "MenuController.h"
#include "../Sound/SoundEngine.h"
#include "../GameLogic/GameLogic.h"

PauseController::PauseController()
{
	m_renderer = dynamic_cast<PauseRenderer*>(RenderEngine::instance().getRenderer("pause"));
}

PauseController::~PauseController()
{
}



void PauseController :: activate()
{
	// We keep all renderers (world/hud) active and add pause on top of it
	RenderEngine& re = RenderEngine::safeInstance();
	re.activateRenderer("pause");
}

void PauseController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateRenderer("pause");
}

void PauseController :: update(float dt)
{
	m_renderer->update(dt);
	SoundEngine::instance().update();

	Input& input = Input::instance();

	// ESC pressed --> back to game
	if (input.isKeyGoingDown(27))
	{
		ControllerManager::instance().activateController(new GameController());
	}

	// ENTER pressed --> unload level and go back to main menu
	if (input.isKeyGoingDown(13))
	{
		LoadingController* lc = new LoadingController();
		lc->setLoader(this, &PauseController::_loadMainMenu);
		ControllerManager::instance().activateController(lc);
	}
}

void PauseController :: _loadMainMenu()
{
	GameLogic::instance().unloadLevel();
	ControllerManager::instance().activateController(new MenuController());
}
