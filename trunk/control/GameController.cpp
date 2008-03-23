//*****************************************************************************
//**
//**   File:               GameController.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 01:41:43 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 203 $
//**
//*****************************************************************************


#include "GameController.h"
#include "../rendering/RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include "../AI/AIEngine.h"
#include "../physics/PhysicsEngine.h"
#include "OSinterface/Input.h"
#include "../math/Vector3.h"
#include "../math/maths.h"

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
	re.activateRenderer("hud");

	// Hack to get things working
	// TODO: think about control flow to fix this

	GameLogic::safeInstance().loadLevel("TestLevel");
}

void GameController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
	// Hack again?
	//GameLogic::safeInstance().unloadLevel();
}


inline Point3 getMouseMapPosition(float height) {
	return RenderEngine::instance().getMapPositionFromScreenPosition(Input::instance().getMousePosition(), height);
}

inline Point3 getMouseMapPosition() {
	return getMouseMapPosition(GameLogic::instance().getGamePlaneHeight());
}

#include "../math/Rotation.h"
#include "../math/Vector3.h"

void GameController :: update(float dt)
{
	GameLogic& gl = GameLogic::instance();

	// Hardcoded the keys for now: WASD
	Vector3 direction;
	direction.set(0.0f, 0.0f, 0.0f);
	Input& input = Input::instance();
	if (input.isKeyDown('W') || input.isKeyGoingDown('W'))
		direction += Vector3(0.0f, 0.0f, -1.0f);
	if (input.isKeyDown('A') || input.isKeyGoingDown('A'))
		direction += Vector3(-1.0f, 0.0f, 0.0f);
	if (input.isKeyDown('S') || input.isKeyGoingDown('S'))
		direction += Vector3(0.0f, 0.0f, 1.0f);
	if (input.isKeyDown('D') || input.isKeyGoingDown('D'))
		direction += Vector3(1.0f, 0.0f, 0.0f);

	if (!Math::float_is_zero(direction.lengthSquared())) {
		direction.normalize();
		gl.setPlayerDirection(direction);
	}
	else {
		// if no direction is pressed on keyboard, turn off thruster, but leave direction the same
		gl.setPlayerThrusterPower(0.0f);
	}



	if (input.isMouseButtonDown(1) || input.isMouseButtonGoingDown(1)) {
		gl.firePositiveLaser(getMouseMapPosition());
	}
		
	if (input.isMouseButtonDown(2) || input.isMouseButtonGoingDown(2)) {
		gl.fireNegativeLaser(getMouseMapPosition());
	}

	if (input.isKeyDown(32)) {
		Point3 pos = getMouseMapPosition();
		gl.dropEbomb(getMouseMapPosition(gl.getTerrainHeight(pos.getX(), pos.getY())));
	}


	AIEngine::instance().update(dt);
	PhysicsEngine::instance().update(dt);
	gl.update(dt);
	RenderEngine::instance().update(dt);
}
