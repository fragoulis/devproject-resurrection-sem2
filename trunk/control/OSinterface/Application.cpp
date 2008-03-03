#include "Application.h"
#include "../../gfxutils/ConfParser/ConfParser.h"
#include "../../gfxutils/MemManager/MemMgr_RawData.h"
#include "../ControllerManager.h"
#include "../GameController.h"
#include "../../GameLogic/GameLogic.h"
#include "../../rendering/RenderEngine.h"
#include "../../sound/SoundEngine.h"
#include "../../AI/AIEngine.h"
#include "../../Physics/PhysicsEngine.h"
#include "../../GameLogic/EnemyTypes.h"
#include "Input.h"

const float DELTA_TIME_MAX = 0.1f;

Application :: Application()
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

	ConfParser cp("./config/config.txt");
	const ParserSection& ps = cp.rootSection();

	MemMgrRaw::init(ps.getSection("MemManager:RawData"));

	EnemyTypes::safeInstance().onApplicationLoad(ps);
	GameLogic::safeInstance().onApplicationLoad(ps);
	SoundEngine::safeInstance().onApplicationLoad(ps);
	RenderEngine::safeInstance().onApplicationLoad(ps);
	AIEngine::safeInstance().onApplicationLoad(ps);
	PhysicsEngine::safeInstance().onApplicationLoad(ps);

	cm.activateController("game");

	return true;
}

void Application :: destroy()
{
	// TODO: rethink program flow
	// would be nice to show "Unloading" or "Exiting" screen


	ControllerManager::safeInstance().unloadAllControllers();
	RenderEngine::safeInstance().unloadAllRenderers();

	EnemyTypes::destroy();
	PhysicsEngine::destroy();
	AIEngine::destroy();
	SoundEngine::destroy();
	ControllerManager::destroy();
	RenderEngine::destroy();
	SoundEngine::destroy();
	GameLogic::destroy();
	MemMgrRaw::destroy();
}

void Application :: render(Graphics& g) const
{
	RenderEngine::instance().render(g);
}
void Application :: update(float dt)
{
	if (dt > DELTA_TIME_MAX) dt = DELTA_TIME_MAX;
	ControllerManager::instance().update(dt);
	Input::instance().update();
}
