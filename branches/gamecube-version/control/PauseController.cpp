#include "PauseController.h"
#include "../rendering/RenderEngine.h"
#include "../rendering/PauseRenderer.h"
#include "ControllerManager.h"
#include "OSinterface/Input.h"
#include "LoadingController.h"
#include "GameController.h"
#include "MenuController.h"
#include "../Sound/SoundEngine.h"
#include "../GameLogic/GameLogic.h"

PauseController::PauseController()
{
}

PauseController::~PauseController()
{
}



void PauseController :: activate()
{
	// We keep all renderers (world/hud) active and add pause on top of it
	RenderEngine& re = RenderEngine::safeInstance();
	re.activateRenderer("pause");
	m_renderer = static_cast<PauseRenderer*>(RenderEngine::instance().getRenderer("pause"));
	m_renderer->setSelectedItem(0);

    //SoundEngine::instance().clearSoundPositions();
}

void PauseController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateRenderer("pause");
}

void PauseController :: update(float dt)
{
	m_renderer->update(dt);

	Input& input = Input::instance();

	if (!input.hasError(0))
	{

		// items selection
		if (input.isControlUp(0)/* || input.isControlLeft(0)*/)
		{
			m_renderer->selectPreviousItem();
			SoundEngine::instance().play("IFaceClick");
		}

		if (input.isControlDown(0)/* || input.isControlRight(0)*/)
		{
			m_renderer->selectNextItem();
			SoundEngine::instance().play("IFaceClick");
		}


		// Pressing A button
		if (input.isButtonGoingDown(0, PAD_BUTTON_A))
		{
			switch (m_renderer->getState())
			{
				case PauseRenderer::MENU_STATE_PAUSE:
				{
					if (m_renderer->getSelectedItem() == 0) {  //resume game
						GameController::instance().continueGame();
					//} else if (m_renderer->getSelectedItem() == 1) {  //go to instructions screen
					//	m_renderer->setState(PauseRenderer::MENU_STATE_INSTRUCTIONS);  
					} else if (m_renderer->getSelectedItem() == 1) {  //go back to main menu
						LoadingController::instance().load(this, &PauseController::_loadMainMenu);
					}
					SoundEngine::instance().play("EnemyFighter_Destroyed");
					break;
				}
				case PauseRenderer::MENU_STATE_INSTRUCTIONS:
				{
					m_renderer->setState(PauseRenderer::MENU_STATE_PAUSE);  //go back to the main pause screen
					SoundEngine::instance().play("EnemyFighter_Destroyed");
					break;
				}

				default:
				{
					break;
				}
			}
		}
	}






	//if (input.isKeyGoingDown(KEY_UP) || input.isKeyGoingDown(KEY_LEFT) ||
	//	input.isKeyGoingDown('W') || input.isKeyGoingDown('A'))
	//{
	//	m_renderer->selectPreviousItem();
	//	SoundEngine::instance().play("MenuClick");
	//}
	//if (input.isKeyGoingDown(KEY_DOWN) || input.isKeyGoingDown(KEY_RIGHT) ||
	//	input.isKeyGoingDown('S') || input.isKeyGoingDown('D'))
	//{
	//	m_renderer->selectNextItem();
	//	SoundEngine::instance().play("MenuClick");
	//}

	////ENTER
	//if (input.isKeyGoingDown(KEY_ENTER)) 
 //   {
	//	switch (m_renderer->getState()) 
 //       {
	//		case PauseRenderer::MENU_STATE_PAUSE:
	//			if (m_renderer->getSelectedItem() == 0) {  //resume game
	//				GameController::instance().continueGame();
	//			} else if (m_renderer->getSelectedItem() == 1) {  //go to instructions screen
	//				m_renderer->setState(PauseRenderer::MENU_STATE_INSTRUCTIONS);  
	//			} else if (m_renderer->getSelectedItem() == 2) {  //go back to main menu
	//				LoadingController::instance().load(this, &PauseController::_loadMainMenu);
	//			}
	//			SoundEngine::instance().play("EnemyFighter_Destroyed");

	//			break;
	//		case PauseRenderer::MENU_STATE_INSTRUCTIONS:
	//			m_renderer->setState(PauseRenderer::MENU_STATE_PAUSE);  //go back to the main pause screen
	//			SoundEngine::instance().play("EnemyFighter_Destroyed");

	//			break;
 //       } // switch ( )
	//}

	//if (input.isKeyGoingDown(KEY_ESC))
	//{
	//	switch (m_renderer->getState()) 
 //       {
	//		case PauseRenderer::MENU_STATE_PAUSE:
	//			GameController::instance().continueGame();
	//			break;
	//		case PauseRenderer::MENU_STATE_INSTRUCTIONS:
	//			m_renderer->setState(PauseRenderer::MENU_STATE_PAUSE);  //go back to the main pause screen
	//			SoundEngine::instance().play("EnemyFighter_Destroyed");
	//			break;
	//	}
	//}
}

void PauseController :: _loadMainMenu()
{
	GameLogic::instance().unloadLevel();
	MenuController::instance().load();
}
