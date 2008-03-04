#include "GameController.h"
#include "../rendering/RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include "../AI/AIEngine.h"
#include "../physics/PhysicsEngine.h"
#include "OSinterface/Input.h"
#include "../math/Vector3.h"
#include "../GameLogic/Lasers/LaserTypes.h"

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
	if (input.isKeyDown('W') || input.isKeyGoingDown('W'))
		direction += Vector3(0.0f, 1.0f, 0.0f);
	if (input.isKeyDown('A') || input.isKeyGoingDown('A'))
		direction += Vector3(-1.0f, 0.0f, 0.0f);
	if (input.isKeyDown('S') || input.isKeyGoingDown('S'))
		direction += Vector3(0.0f, -1.0f, 0.0f);
	if (input.isKeyDown('D') || input.isKeyGoingDown('D'))
		direction += Vector3(1.0f, 0.0f, 0.0f);

	direction.normalize();

	GameLogic& gl = GameLogic::instance();
	gl.setPlayerDirection(direction);


	if (input.isMouseButtonDown(0) || input.isMouseButtonGoingDown(0)) _fireLaser(PLAYER_POSITIVE);
	if (input.isMouseButtonDown(1) || input.isMouseButtonGoingDown(1)) _fireLaser(PLAYER_NEGATIVE);

	gl.update(dt);

	AIEngine::instance().update(dt);
	PhysicsEngine::instance().update(dt);
	RenderEngine::instance().update(dt);
}

void GameController :: _fireLaser(LaserType type)
{
	Point3 mapPosition = RenderEngine::instance().getMapPositionFromScreenPosition(Input::instance().getMousePosition());
	GameLogic::instance().fireLaser(mapPosition, type);
}