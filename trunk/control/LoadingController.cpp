#include "LoadingController.h"
#include "../rendering/RenderEngine.h"

LoadingController :: LoadingController()
{
}

LoadingController :: ~LoadingController()
{
}


void LoadingController :: activate()
{
	// deactivate all user input, activate loading renderer
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateAllRenderers();
	re.activateRenderer("loading");
}

void LoadingController :: deactivate()
{
	RenderEngine& re = RenderEngine::instance();
	re.deactivateAllRenderers();
}

void LoadingController :: update(float dt)
{
	// animate the loading screen!
	// maybe do that in the renderer itself
	// that means IRenderer needs an update method
}
