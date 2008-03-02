#include "GameController.h"
#include "../rendering/RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include "../AI/AIEngine.h"
#include "../physics/PhysicsEngine.h"
#include "OSinterface/Input.h"
#include "../math/Vector3.h"

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
	// Hardcoded the keys for now: WASD
	Vector3 direction;
	direction.set(0.0f, 0.0f, 0.0f);
	Input& input = Input::instance();
	if (input.isKeyDown('w') || input.isKeyGoingDown('w'))
		direction += Vector3(0.0f, 1.0f, 0.0f);
	if (input.isKeyDown('a') || input.isKeyGoingDown('a'))
		direction += Vector3(-1.0f, 0.0f, 0.0f);
	if (input.isKeyDown('s') || input.isKeyGoingDown('s'))
		direction += Vector3(0.0f, -1.0f, 0.0f);
	if (input.isKeyDown('d') || input.isKeyGoingDown('d'))
		direction += Vector3(1.0f, 0.0f, 0.0f);

	direction.normalize();

	GameLogic& gl = GameLogic::instance();
	gl.setPlayerDirection(direction);

	gl.update(dt);

	AIEngine::instance().update(dt);
	PhysicsEngine::instance().update(dt);
	RenderEngine::instance().update(dt);
}
