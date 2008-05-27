#include "GameOverController.h"
#include "MenuController.h"
#include "LoadingController.h"
#include "ControllerManager.h"
#include "../rendering/RenderEngine.h"
#include "../Sound/SoundEngine.h"
#include "OSinterface/Input.h"
#include "../GameLogic/GameLogic.h"

GameOverController::GameOverController()
{
}

GameOverController::~GameOverController()
{
}

void GameOverController :: activate()
{
	RenderEngine::instance().activateRenderer("GameOver");
    //SoundEngine::instance().clearSoundPositions();
}

void GameOverController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
	RenderEngine::instance().deactivateRenderer("GameOver");
}


void GameOverController :: update(float dt)
{
	Input& input = Input::instance();

	if (!input.hasError(0) && input.isButtonGoingDown(0, PAD_BUTTON_A))
	{
		LoadingController::instance().load(this, &GameOverController::_loadMainMenu);
	}

	RenderEngine::instance().update(dt);
}


void GameOverController :: _loadMainMenu()
{
	GameLogic::instance().unloadLevel();
	RenderEngine::instance().unloadAllRenderers();
	MenuController::instance().load();
}