#include "PlanetController.h"
#include "../rendering/RenderEngine.h"
#include "../rendering/PlanetRenderer.h"
#include "ControllerManager.h"
#include "OSinterface/Input.h"
#include "LoadingController.h"
#include "../Sound/SoundEngine.h"

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
	re.deactivateAllRenderers();
	re.activateRenderer("planet");
}

void PlanetController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateRenderer("planet");
}

void PlanetController :: update(float dt)
{
	m_renderer->update(dt);
	//SoundEngine::instance().update();


	Input& input = Input::instance();

	if (input.isKeyDownOrGoingDown(27))
	{
		LoadingController* lc = new LoadingController();
		// TODO!
	}
}
