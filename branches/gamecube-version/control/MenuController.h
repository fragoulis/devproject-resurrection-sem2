#ifndef _RES_MENU_CONTROLLER_H_
#define _RES_MENU_CONTROLLER_H_
#include "IController.h"
#include "../utility/Singleton.h"
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include <vector>
class MenuRenderer;
class ParserSection;




/**
 * Sets up the main menu and handles user input for it
 *
 */
class MenuController :
	public IController,
	public Singleton< MenuController >,
	public EventListener<Level_Complete>
{
public:
	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);


	// Helper to load menu
	void load();
	void loadPlanetSelection();

	virtual void onEvent(Level_Complete&);

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();


private:
	MenuRenderer* m_renderer;
	// Menu data
    
    // ambient sound id
    unsigned m_ambientSound;

	typedef std::vector<std::string> StringVector;
	StringVector m_levelNames;

	std::string m_levelToLoad;
	void _startLevel(const std::string& id);
	void _exit();

	// Callback for LoadingController
	// New game simply loads the very first level
	void _loadLevel();


	// Singleton stuffz
	friend Singleton< MenuController >;
	MenuController();
	virtual ~MenuController();
};

#endif