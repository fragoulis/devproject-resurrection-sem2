//*****************************************************************************
//**
//**   File:               Application.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


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
#include "../../GameLogic/Enemies/EnemyFactory.h"
#include "../../GameLogic/Lasers/LaserFactory.h"
#include "../../gfxutils/Misc/Logger.h"
#include "../../GameLogic/WorldObjectTypeManager.h"
#include "Input.h"
#include <iostream>
#include <fstream>
using namespace std;

const float DELTA_TIME_MAX = 0.1f;
const float DELTA_TIME_MIN = 0.01f;

Application :: Application()
{
}

Application :: ~Application()
{
}


ofstream filestr;

bool Application :: init()
{
	streambuf* psbuf;
	filestr.open("cout.txt");
	psbuf = filestr.rdbuf();
	cout.rdbuf(psbuf);

	ControllerManager& cm = ControllerManager::safeInstance();
	cm.activateController("loading");

	// this blocks the loading screen from showing
	// should fire a new thread that does some loadController and loadRenderers
	// but that doesn't work cuz loading needs rendering context
	// so need to rethink how controllers and renderers and stuff are loaded and managed
	// will see

	ConfParser cp("./config/config.txt");
	const ParserSection& ps = cp.rootSection();

	Logger::init(ps.getSection("Logger"));
	MemMgrRaw::init(ps.getSection("MemManager:RawData"));

	WorldObjectTypeManager::safeInstance().onApplicationLoad(ps);
	LaserFactory::safeInstance().onApplicationLoad(ps);
	EnemyFactory::safeInstance().onApplicationLoad(ps);
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

	WorldObjectTypeManager::destroy();
	LaserFactory::destroy();
	EnemyFactory::destroy();
	PhysicsEngine::destroy();
	AIEngine::destroy();
	SoundEngine::destroy();
	ControllerManager::destroy();
	RenderEngine::destroy();
	SoundEngine::destroy();
	GameLogic::destroy();
	MemMgrRaw::destroy();
	Logger::destroy();

	filestr.close();
}

void Application :: render(Graphics& g) const
{
	RenderEngine::instance().render(g);
}
void Application :: update(float dt)
{
	if (dt > DELTA_TIME_MAX) dt = DELTA_TIME_MAX;
	//if (dt < DELTA_TIME_MIN) dt = DELTA_TIME_MIN;		<--- BAD!! 
	Input::instance().update();
	ControllerManager::instance().update(dt);
}
