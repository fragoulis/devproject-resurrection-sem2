#include "GameController.h"
#include "ControllerManager.h"
#include "PauseController.h"
#include "LoadingController.h"
//#include "../rendering/RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include "../Gamelogic/Buffs/EnemyBuffCoupleManager.h"
#include "../AI/AIEngine.h"
#include "../physics/PhysicsEngine.h"
//#include "../Sound/SoundEngine.h"
#include "OSinterface/Input.h"
#include "../math/Vector3.h"
#include "../math/maths.h"
#include "../math/Rotation.h"
#include <iostream>
using namespace std;

GameController :: GameController()
{
	EventManager::instance().registerEventListener<Level_Complete>(this);
	EventManager::instance().registerEventListener<Game_Over>(this);
}

GameController :: ~GameController()
{
}


void GameController :: activate()
{
	//RenderEngine& re = RenderEngine::safeInstance();
	//re.deactivateAllRenderers();
	//re.activateRenderer("world");
	//re.activateRenderer("hud");
}

void GameController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
}


//inline Point3 getMouseMapPosition(float height) {
	//return RenderEngine::instance().getMapPositionFromScreenPosition(Input::instance().getMousePosition(), height);
//}

//inline Point3 getMouseMapPosition() {
//	return getMouseMapPosition(GameLogic::instance().getGamePlaneHeight());
//}


void GameController :: update(float dt)
{
	GameLogic& gl = GameLogic::instance();

	// Hardcoded the keys for now: WASD
	Vector3 direction;
	direction.set(0.0f, 0.0f, 0.0f);
	Input& input = Input::instance();

	if (!input.hasError(0)) {
		int controlX = input.getControlX(0);
		int controlY = input.getControlY(0);
		if (controlX != 0 || controlY != 0)
		{
			direction.setX(float(controlX) / 56.0f);
			direction.setZ(float(controlY) / 56.0f);
			//direction.normalize();
			//cout << "dir: " << direction << endl;
			gl.setPlayerDirection(direction);
		}
		else
		{
			gl.setPlayerThrusterPower(0.0f);
		}
	}
	else
	{
		gl.setPlayerThrusterPower(0.0f);
	}


	//if (input.isKeyDownOrGoingDown('P'))
	//	RenderEngine::instance().adjustCamera(5.0f);
	//if (input.isKeyDownOrGoingDown('O'))
	//	RenderEngine::instance().adjustCamera(-5.0f);

	//if (input.isKeyGoingDown('N'))
	//	RenderEngine::instance().toggleWireframe();

	//if (input.isKeyGoingDown('M'))
	//	RenderEngine::instance().togglePostProc();




	//// Left mouse --> fire positive laser
	//if (input.isMouseButtonDown(1) || input.isMouseButtonGoingDown(1)) {
	//	gl.firePositiveLaser(getMouseMapPosition());
	//}
	//// Right mouse --> fire negative laser
	//if (input.isMouseButtonDown(2) || input.isMouseButtonGoingDown(2)) {
	//	gl.fireNegativeLaser(getMouseMapPosition());
	//}

	//// SPACE --> drop e-bomb
	//if (input.isKeyGoingDown(32)) {
	//	Point3 pos = getMouseMapPosition();
	//	gl.dropEbomb(getMouseMapPosition(gl.getTerrainHeight(pos.getX(), pos.getY())));
	//}

	//// ESC --> pause game
	//if (input.isKeyGoingDown(27))
	//{
	//	ControllerManager::instance().activateController(&m_pauseController);
	//}


	//SoundEngine::instance().update();
	AIEngine::instance().update(dt);
    EnemyBuffCoupleManager::instance().update();
	PhysicsEngine::instance().update(dt);
	//RenderEngine::instance().update(dt);
	gl.update(dt);
}


void GameController :: onEvent(Level_Complete&)
{
	ControllerManager::instance().activateController(&m_levelCompleteController);
}

void GameController :: onEvent(Game_Over&)
{
	ControllerManager::instance().activateController(&m_gameOverController);
}

void GameController :: loadLevel(const std::string& levelName)
{
	//RenderEngine::instance().loadRenderer("world");
	//RenderEngine::instance().loadRenderer("hud");
	GameLogic::safeInstance().loadLevel(levelName);
	ControllerManager::instance().activateController(this);
}

void GameController :: continueGame()
{
	ControllerManager::instance().activateController(this);
}
