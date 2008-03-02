#include "GameController.h"
#include "../rendering/RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include "../AI/AIEngine.h"
#include "../physics/PhysicsEngine.h"

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

	GameLogic::safeInstance().loadLevel("TestLevel");
}

void GameController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
}

void GameController :: update(float dt)
{
	GameLogic::instance().update(dt);
	AIEngine::instance().update(dt);
	PhysicsEngine::instance().update(dt);
	RenderEngine::instance().update(dt);
}
