//*****************************************************************************
//**
//**   File:               MenuController.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 01:41:43 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 203 $
//**
//*****************************************************************************

#include "MenuController.h"
#include "../rendering/RenderEngine.h"
#include "../rendering/MenuRenderer.h"
#include "ControllerManager.h"
#include "OSinterface/Input.h"
#include "LoadingController.h"
#include "GameController.h"
#include "OSinterface/IOSInterface.h"
#include "../GameLogic/GameLogic.h"
#include "../Sound/SoundEngine.h"


MenuController::MenuController()
{
	EventManager::instance().registerEventListener<Level_Complete>(this);
}

MenuController :: ~MenuController()
{
}


void MenuController :: activate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateAllRenderers();
	re.activateRenderer("menu");
	m_renderer = dynamic_cast<MenuRenderer*>(RenderEngine::instance().getRenderer("menu"));
    
    SoundEngine::instance().clearSoundPositions();
    m_ambientSound = SoundEngine::instance().play("InterfaceAmbient", true);
}

void MenuController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateAllRenderers();

    SoundEngine::instance().stop( m_ambientSound );
}

void MenuController :: update(float dt)
{
	// check user input here
	// if something in menu needs to change, update data
	// and ask MenuRenderer to change the graphics.
	// if you want to exit the menu back to the game:
	// ControllerManager::instance().activateController("game")
	// GameController is currently hacked to load the TestLevel when it is activated
	// Later on we shall have to have communication between controllers hmm...


	// if you want the game to run: GameLogic::instance().update(dt)
	// if you want the graphics to update: RenderEngine::instance().update(dt)
	// problem: can't have some renderers update and others not ATM
	// this design sucks!

	// do updates here
	m_renderer->update(dt);

	Input& input = Input::instance();
    
	//items selection
	if (input.isKeyGoingDown(KEY_UP) || input.isKeyGoingDown(KEY_LEFT) ||
		input.isKeyGoingDown('W') || input.isKeyGoingDown('A')) {
		if (m_renderer->selectPreviousItem())
			SoundEngine::instance().play("MenuClick");
	}
	if (input.isKeyGoingDown(KEY_DOWN) || input.isKeyGoingDown(KEY_RIGHT) ||
		input.isKeyGoingDown('S') || input.isKeyGoingDown('D'))
	{
		if (m_renderer->selectNextItem())
			SoundEngine::instance().play("MenuClick");
	}

	//ENTER
	if (input.isKeyGoingDown(KEY_ENTER)) 
    {
		switch (m_renderer->getState()) 
        {
			case MenuRenderer::MENU_STATE_MAIN:
				if (m_renderer->getSelectedItem() == 0) {
					m_renderer->setState(MenuRenderer::MENU_STATE_PLANET);  //go to planet screen selection
                } else if (m_renderer->getSelectedItem() == 1) {
					m_renderer->setState(MenuRenderer::MENU_STATE_TUTORIAL);  //go to credits screen
				} else if (m_renderer->getSelectedItem() == 2) {
					m_renderer->setState(MenuRenderer::MENU_STATE_CREDITS);  //go to credits screen
				} else if (m_renderer->getSelectedItem() == 3) {
					_exit();  //kill the application
				}
				SoundEngine::instance().play("EnemyFighter_Destroyed");

				break;
			case MenuRenderer::MENU_STATE_TUTORIAL:
				_newGame();  //go to new game
				m_renderer->setSelectedItem(1); //select the first planet for the next visit
				SoundEngine::instance().play("EnemyFighter_Destroyed");

				break;
			case MenuRenderer::MENU_STATE_CREDITS:
				m_renderer->setState(MenuRenderer::MENU_STATE_MAIN);  //go back to the main menu 
				SoundEngine::instance().play("EnemyFighter_Destroyed");

				break;
			case MenuRenderer::MENU_STATE_PLANET:
				if (m_renderer->getSelectedItem() == 0)
					m_renderer->setState(MenuRenderer::MENU_STATE_MAIN);  //go back to the main menu
				else 
					_newGame();  //go to new game

				m_renderer->setSelectedItem(4); //select the first planet for the next visit
				SoundEngine::instance().play("EnemyFighter_Destroyed");

				break;
        } // switch ( )
	}

	if (input.isKeyGoingDown(KEY_ESC))
	{
		switch (m_renderer->getState()) 
        {
			case MenuRenderer::MENU_STATE_MAIN:
				_exit();
				break;
			case MenuRenderer::MENU_STATE_TUTORIAL:
			case MenuRenderer::MENU_STATE_CREDITS:
			case MenuRenderer::MENU_STATE_PLANET:
				m_renderer->setState(MenuRenderer::MENU_STATE_MAIN);
				break;
		};
	}



} // update()

void MenuController :: _newGame()
{
	LoadingController::instance().load(this, &MenuController::_loadFirstLevel);
}

void MenuController :: _exit()
{
	IOSInterface* os = IOSInterface::instance();
	os->quit();
}

void MenuController :: _loadFirstLevel()
{
	RenderEngine::instance().unloadAllRenderers();
	//RenderEngine::instance().unloadRenderer("menu");
	GameController::safeInstance().loadLevel("TestLevel");
}

void MenuController :: load()
{
	//LoadingController& lc = LoadingController::instance();
	//lc.setLoader(this, &MenuController::_loadMenu);
	//RenderEngine::unloadAllRenderers();
	RenderEngine::instance().loadRenderer("menu");
	ControllerManager::instance().activateController(this);
}

void MenuController :: loadPlanetSelection()
{
	RenderEngine::instance().loadRenderer("menu");
	m_renderer = dynamic_cast<MenuRenderer*>(RenderEngine::instance().getRenderer("menu"));
	m_renderer->setState(MenuRenderer::MENU_STATE_PLANET);
	ControllerManager::instance().activateController(this);
}

//void MenuController :: _loadMenu()
//{
//	GameLogic::instance().unloadLevel();
//	ControllerManager::instance().activateController(new MenuController());
//}

void MenuController :: onEvent(Level_Complete& level) {

	string levelName = level.getValue();

	//TODO: get the level names and check when they're completed

}
