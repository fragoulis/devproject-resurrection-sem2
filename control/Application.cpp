#include "Application.h"
#include "ControllerManager.h"
#include "GameController.h"
#include "../GameLogic/GameplayLogic.h"
#include "../rendering/RenderEngine.h"
#include "Input.h"


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
	// but that doesn't work cuz loading needs rendering context
	// so need to rethink how controllers and renderers and stuff are loaded and managed
	// will see
	cm.activateController("game");

	return true;
}

void Application :: destroy()
{
	ControllerManager::safeInstance().unloadAllControllers();
	RenderEngine::safeInstance().unloadAllRenderers();

	ControllerManager::destroy();
	RenderEngine::destroy();
	GameplayLogic::destroy();
}

void Application :: render(Graphics& g) const
{
	RenderEngine::instance().render(g);
}
void Application :: update(float dt)
{
	ControllerManager::instance().update(dt);
	RenderEngine::instance().update(dt);
	Input::instance().update();
}
