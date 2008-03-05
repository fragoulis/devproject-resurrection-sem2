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

	//! Renders stuffz!
	void render(Graphics& g) const;

	//! Updates stuffz!
	void update(float dt);
};
