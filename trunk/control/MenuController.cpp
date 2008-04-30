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
				SoundEngine::instance().play("MenuClick");

				break;
			case MenuRenderer::MENU_STATE_TUTORIAL:
				_startLevel("Tutorial01");  //go to new game
				m_renderer->setSelectedItem(1); //select the first planet for the next visit
				SoundEngine::instance().play("MenuClick");

				break;
			case MenuRenderer::MENU_STATE_CREDITS:
				m_renderer->setState(MenuRenderer::MENU_STATE_MAIN);  //go back to the main menu 
				SoundEngine::instance().play("MenuClick");

				break;
			case MenuRenderer::MENU_STATE_PLANET:
				if (m_renderer->getSelectedItem() == 0)
					m_renderer->setState(MenuRenderer::MENU_STATE_MAIN);  //go back to the main menu
				else
				{
					int level = m_renderer->getSelectedItem() - 3;
					std::string id = "Level0";
					id += (level + 48);
					_startLevel(id);
				}

				m_renderer->setSelectedItem(4); //select the first planet for the next visit
				SoundEngine::instance().play("MenuClick");

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
        SoundEngine::instance().play("MenuClick");
	}



} // update()

void MenuController :: _startLevel(const std::string& id)
{
	m_levelToLoad = id;
	LoadingController::instance().load<>(this, &MenuController::_loadLevel);
}

void MenuController :: _exit()
{
	IOSInterface* os = IOSInterface::instance();
	os->quit();
}

void MenuController :: _loadLevel()
{
	RenderEngine::instance().unloadAllRenderers();
	//RenderEngine::instance().unloadRenderer("menu");
	//GameController::safeInstance().loadLevel("TestLevel");
    //GameController::safeInstance().loadLevel("Level01");
    GameController::safeInstance().loadLevel(m_levelToLoad);
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
