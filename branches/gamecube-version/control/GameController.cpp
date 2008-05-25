#include "GameController.h"
#include "ControllerManager.h"
#include "PauseController.h"
#include "LoadingController.h"
#include "../rendering/RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include "../Gamelogic/Buffs/EnemyBuffCoupleManager.h"
#include "../AI/AIEngine.h"
#include "../physics/PhysicsEngine.h"
//#include "../Sound/SoundEngine.h"
#include "OSinterface/Input.h"
#include "../math/Vector3.h"
#include "../math/maths.h"
#include "../math/Rotation.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include <iostream>
using namespace std;

GameController :: GameController()
{
	EventManager::instance().registerEventListener<Level_Complete>(this);
	EventManager::instance().registerEventListener<Game_Over>(this);
	//EventManager::instance().registerEventListener<Button_GoingDown>(this);
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
}

void GameController :: deactivate()
{
	// deactivate controls, but leaves renderers alone
}

//void GameController :: onEvent(Button_GoingDown& evt)
//{
//	cout << "Button down: " << evt.getValue() << endl;
//}



void GameController :: update(float dt)
{
	GameLogic& gl = GameLogic::instance();
	static int laserPositiveType = gl.getPositiveLaserType();
	static int laserNegativeType = gl.getNegativeLaserType();
	static int laserFire = laserPositiveType;

	// Hardcoded the keys for now: WASD
	Input& input = Input::instance();

	if (!input.hasError(0))
	{
		// swap laser with L trigger
		if (input.isTriggerLGoingDown(0))
		{
			laserFire = (laserFire == laserPositiveType ? laserNegativeType : laserPositiveType);
			CKLOG(std::string("Laser fire swapped to ") + ToString<int>(laserFire), 3);
		}

		// get control stick input --> player movement
		int controlX = -input.getControlX(0);
		int controlY = -input.getControlY(0);
		if (controlX != 0 || controlY != 0)
		{
			Vector3 direction(float(controlX) / 56.0f, 0.0f, float(controlY) / 56.0f);
			gl.setPlayerDirection(direction);
		}
		else
		{
			gl.setPlayerThrusterPower(0.0f);
		}

		// get C-stick input --> laser fire
		int camX = -input.getCameraX(0);
		int camY = -input.getCameraY(0);
		if (camX != 0 || camY != 0)
		{
			Vector3 direction(float(camX) / 44.0f, 0.0f, float(camY) / 44.0f);
			direction.normalize();
			gl.fireLaser(direction, laserFire);
		}

		// Drop E-bomb
		if (input.isTriggerRGoingDown(0))
		{
			gl.dropEbomb();
		}
	}
	else // if (!input.hasError(0))
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
	RenderEngine::safeInstance().loadRenderer("world");
	RenderEngine::instance().loadRenderer("hud");
	GameLogic::safeInstance().loadLevel(levelName);
	ControllerManager::instance().activateController(this);
}

void GameController :: continueGame()
{
	ControllerManager::instance().activateController(this);
}
