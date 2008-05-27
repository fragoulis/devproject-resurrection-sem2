#include "LevelCompleteController.h"
#include "MenuController.h"
#include "LoadingController.h"
#include "ControllerManager.h"
#include "../rendering/RenderEngine.h"
//#include "../Sound/SoundEngine.h"
#include "OSinterface/Input.h"
#include "../GameLogic/GameLogic.h"
#include "../physics/PhysicsEngine.h"

LevelCompleteController::LevelCompleteController()
{
}

LevelCompleteController::~LevelCompleteController()
{
}

void LevelCompleteController :: activate()
{
	// Level complete gfx is rendered directly somewhere else?
	// OR we can add a renderer for it, dunno
	// That IS better :D
	RenderEngine::instance().activateRenderer("LevelComplete");
    //SoundEngine::instance().clearSoundPositions();
}

void LevelCompleteController :: deactivate()
{
	RenderEngine::instance().deactivateRenderer("LevelComplete");
}


void LevelCompleteController :: update(float dt)
{
	Input& input = Input::instance();

	if (!input.hasError(0) && input.isButtonGoingDown(0, PAD_BUTTON_A))
	{
		LoadingController::instance().load(this, &LevelCompleteController::_loadPlanetSelectionScreen);
	}

	RenderEngine::instance().update(dt);


	PhysicsEngine::instance().update(dt);
	RenderEngine::instance().update(dt);
}



void LevelCompleteController :: _loadPlanetSelectionScreen()
{
	GameLogic::instance().unloadLevel();
	RenderEngine::instance().unloadAllRenderers();
	MenuController::safeInstance().loadPlanetSelection();
}
