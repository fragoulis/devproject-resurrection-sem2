#include "Application.h"
#include "ControllerManager.h"
#include "GameController.h"
#include "../GameLogic/GameLogic.h"
#include "../rendering/RenderEngine.h"


Application :: Application() : m_wantToQuit(false)
{
}

Application :: ~Application()
{
}



bool Application :: init()
{
	ControllerManager& cm = ControllerManager::safeInstance();
	cm.activateController("loading");

	// this blocks the loading screen from showing
	// should fire a new thread that does some loadController and loadRenderers
	cm.activateController("game");

	return true;
}

void Application :: destroy()
{
	ControllerManager::safeInstance().unloadAllControllers();
}

void Application :: render(Graphics& g) const
{
	RenderEngine::instance().render(g);
}
void Application :: update()
{
	ControllerManager::instance().update();
}
