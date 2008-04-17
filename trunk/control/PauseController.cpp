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
}

PauseController::~PauseController()
{
}



void PauseController :: activate()
{
	// We keep all renderers (world/hud) active and add pause on top of it
	RenderEngine& re = RenderEngine::safeInstance();
	re.activateRenderer("pause");
	m_renderer = dynamic_cast<PauseRenderer*>(RenderEngine::instance().getRenderer("pause"));

    SoundEngine::instance().clearSoundPositions();
}

void PauseController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateRenderer("pause");
}

void PauseController :: update(float dt)
{
	m_renderer->update(dt);

	Input& input = Input::instance();

	if (input.isKeyGoingDown('W')) {
		m_renderer->selectPreviousItem();
	}
	if (input.isKeyGoingDown('S')) {
		m_renderer->selectNextItem();
	}

	//ENTER
	if (input.isKeyGoingDown(13)) {
		if (m_renderer->getSelectedItem() == 0) {  //resume game
			GameController::instance().continueGame();
		} else if (m_renderer->getSelectedItem() == 1) {  //go back to main menu
			LoadingController::instance().load(this, &PauseController::_loadMainMenu);
		}
	}
}

void PauseController :: _loadMainMenu()
{
	GameLogic::instance().unloadLevel();
	RenderEngine::instance().unloadAllRenderers();
	//RenderEngine::instance().unloadRenderer("pause");
	MenuController::instance().load();
}
