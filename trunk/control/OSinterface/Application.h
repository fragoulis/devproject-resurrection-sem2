//*****************************************************************************
//**
//**   File:               Application.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#pragma once
class Graphics;

/**
 * Provides the main interface for the OS to communicate with the game
 * When the program starts, the OS calls Application::init()
 * Every frame Application::render and Application::update are called.
 * When the program quits, the OS calls Application::destroy()
 * The game does not talk to this class.
 *
 * Application::render calls RenderEngine::render
 * Application::update calls Input::update and ControllerManager::update
 * It also restricts deltatime from getting bigger than 0.1 sec
 * Application::init calls onApplicationLoad for every engine
 * Application::destroy deletes all singletons.
 */

class Application
{
public:
	Application();
	~Application();

	//! Initializes the application. Should be called before the first update
	bool init(); // returns false on failure

	//! Frees memory. Should be called after the last update.
	void destroy();

	//! Calls RenderEngine::render
	void render(Graphics& g) const;

	//! Calls ControllerManager::update
	void update(float dt);

	//! Loads up the game. Callback function for LoadingController
	void load();

	//! Unloads the game. Callback function for LoadingController
	void unload();
};
