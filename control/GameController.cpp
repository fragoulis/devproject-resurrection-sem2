#include "GameController.h"
#include "../rendering/RenderEngine.h"
#include "../GameplayLogic/GameplayLogic.h"

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

	// Hack to get things working
	// TODO: think about control flow to fix this

	GameplayLogic::safeInstance().loadLevel("level1");
}

void GameController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
}

void GameController :: update(float dt)
{
	// not much to do here!
}
