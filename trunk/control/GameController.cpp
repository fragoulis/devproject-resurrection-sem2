#include "GameController.h"
#include "../rendering/RenderEngine.h"

GameController :: GameController()
{
}

GameController :: ~GameController()
{
}


void GameController :: activate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateAllRenderers();
	re.activateRenderer("world");
}

void GameController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
}

void GameController :: update()
{
	// not much to do here!
}
