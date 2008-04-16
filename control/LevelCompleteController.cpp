#include "LevelCompleteController.h"
#include "PlanetController.h"
#include "LoadingController.h"
#include "ControllerManager.h"
#include "../rendering/RenderEngine.h"
#include "../Sound/SoundEngine.h"
#include "OSinterface/Input.h"
#include "../GameLogic/GameLogic.h"

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
}

void LevelCompleteController :: deactivate()
{
}


void LevelCompleteController :: update(float dt)
{
	Input& input = Input::instance();

	// ESC or ENTER --> go to planet selection screen
	if (input.isKeyGoingDown(27) || input.isKeyGoingDown(13))
	{
		LoadingController::instance().load(this, &LevelCompleteController::_loadPlanetSelectionScreen);
	}


	SoundEngine::instance().update();
	RenderEngine::instance().update(dt);
}



void LevelCompleteController :: _loadPlanetSelectionScreen()
{
	GameLogic::instance().unloadLevel();
	RenderEngine::instance().unloadAllRenderers();
	PlanetController::safeInstance().load();
}
