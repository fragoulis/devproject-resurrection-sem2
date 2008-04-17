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
	//RenderEngine& re = RenderEngine::safeInstance();
	//re.deactivateAllRenderers();
	//re.activateRenderer("world");
	//re.activateRenderer("hud");
	RenderEngine::instance().activateRenderer("GameOver");
    SoundEngine::instance().clearSoundPositions();
}

void GameOverController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
	RenderEngine::instance().deactivateRenderer("GameOver");
}


void GameOverController :: update(float dt)
{
	Input& input = Input::instance();

	// ESC or ENTER --> go back to main menu
	if (input.isKeyGoingDown(27) || input.isKeyGoingDown(13))
	{
		LoadingController::instance().load(this, &GameOverController::_loadMainMenu);
	}


	// We update render engine so explosions and particles continue
	RenderEngine::instance().update(dt);
}


void GameOverController :: _loadMainMenu()
{
	GameLogic::instance().unloadLevel();
	RenderEngine::instance().unloadAllRenderers();
	MenuController::instance().load();
}