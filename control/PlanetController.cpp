#include "PlanetController.h"
#include "../rendering/RenderEngine.h"
#include "../rendering/PlanetRenderer.h"
#include "ControllerManager.h"
#include "OSinterface/Input.h"
#include "LoadingController.h"
#include "../Sound/SoundEngine.h"
#include <string>

PlanetController::PlanetController()
{
}

PlanetController::~PlanetController()
{
}



void PlanetController :: activate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateAllRenderers();
	re.activateRenderer("planet");
	m_renderer = dynamic_cast<PlanetRenderer*>(RenderEngine::instance().getRenderer("planet"));

    SoundEngine::instance().clearSoundPositions();
}

void PlanetController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateAllRenderers();
}

void PlanetController :: update(float dt)
{
	m_renderer->update(dt);
	//SoundEngine::instance().update();


	Input& input = Input::instance();

	if (input.isKeyDownOrGoingDown(27))
	{
		// TODO!
	}
}


void PlanetController :: load()
{
	RenderEngine::instance().loadRenderer("planet");
	ControllerManager::instance().activateController(this);
}

void PlanetController :: onEvent(Level_Complete& evt)
{
	// Fix planet screen so we set level to complete
	const std::string& levelName = evt.getValue();
}