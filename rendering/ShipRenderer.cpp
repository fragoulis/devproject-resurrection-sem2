#include "ShipRenderer.h"
#include "RenderEngine.h"
#include "../Math/Matrix44.h"
#include "../Math/CoordinateFrame.h"
//#include "../gfx/Model/Model.h"
//#include "../gfx/Model/ModelMgr.h"
//#include "../gfx/Shaders/ShaderManager.h"
//#include "../gfx/Texture/Texture.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Objects/Ebomb.h"
#include "RenderEngine.h"
#include "../GameLogic/WorldObjectTypeManager.h"

#include "../gfxutils/Model/ModelMgr.h"
#include "../gfxutils/Model/Model.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"

#include <iostream>

using namespace std;

//static char texstr[8] = "texmap0";

ShipRenderer :: ShipRenderer()
{
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Despawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_Respawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);
	EventManager::instance().registerEventListener< Enemy_Destroyed >(this);
	EventManager::instance().registerEventListener< Ebomb_Spawned >(this);
	EventManager::instance().registerEventListener< Ebomb_Despawned >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);
}

ShipRenderer :: ~ShipRenderer()
{
	EventManager::instance().unRegisterEventListener< Player_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Player_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Player_Destroyed >(this);
	EventManager::instance().unRegisterEventListener< Player_Respawned >(this);
	EventManager::instance().unRegisterEventListener< Enemy_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Enemy_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Enemy_Destroyed >(this);
	EventManager::instance().unRegisterEventListener< Ebomb_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Ebomb_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Level_Unload >(this);
}

void ShipRenderer :: onEvent(Level_Unload& e)
{
	//m_ships.clear();
}

void ShipRenderer :: render(Graphics& g) const
{
	//Vector4 ldir = RenderEngine::instance().getLevelLight();
	//Vector4 lcol = RenderEngine::instance().getLevelLightColor();

	// SET TEXTURE UNIT 0!!
	
	bool changed_to_tex = false;
	for(vector<CoordinateModel>::const_iterator it = m_ships.begin();
		it != m_ships.end();
		++it)
	{
		//glPushMatrix();
		//glMultMatrixf(it->coordframe->getMatrix().cfp());
		const float * m = it->coordframe->getMatrix().cfp();
	
		

		if(it->model->isTextured())
		{
			if(!changed_to_tex)
			{
				changed_to_tex = true;
				// enable tex unit 0 here
			}
		}
		it->model->render();
	
		//glPopMatrix();
	}
}

void ShipRenderer :: onEvent(Player_Spawned& evt)
{
	// Get the const render settings of the ship
	const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
}

void ShipRenderer :: onEvent(Player_Respawned& evt)
{
	// Get the const render settings of the ship
	const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
	// Throw an effect - ps here
}

void ShipRenderer :: onEvent(Player_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	_deleteShip(cf);
}

void ShipRenderer :: onEvent(Player_Destroyed& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	const CoordinateFrame * cf = &(evt.getValue1()->getCoordinateFrame());
	_deleteShip(cf);
}

void ShipRenderer :: onEvent(Enemy_Spawned& evt)
{
	// Get the const render settings of the ship
	const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
	//CKLOG(string("Active ships : ") + ToString<unsigned>(unsigned(m_ships.size())), 2);
}

void ShipRenderer :: onEvent(Enemy_Despawned& evt)
{
	//CKLOG(string("Despawning ") + ToString<Enemyship*>(evt.getValue()), 3);
	//// Fetch the enemy & remove
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	_deleteShip(cf);
}

void ShipRenderer :: onEvent(Enemy_Destroyed& evt)
{
	//CKLOG(string("Despawning ") + ToString<Enemyship*>(evt.getValue()), 3);
	//// Fetch the enemy & remove
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	_deleteShip(cf);
}

void ShipRenderer :: onEvent(Ebomb_Spawned& evt)
{
	// Get the const render settings of the ship
	const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
}

void ShipRenderer :: onEvent(Ebomb_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	_deleteShip(cf);
}

void ShipRenderer :: _insertShip(const EntitySettings_t& settings, const CoordinateFrame * cframe)
{
	// Arbitrary placement, textured go in the end & untextured in the beginning
	const Model * model = ModelMgr::instance().getModel(settings.modelName);
	if(model->isTextured())
		m_ships.push_back(CoordinateModel(model,cframe));
	else
		m_ships.insert(m_ships.begin(),CoordinateModel(model,cframe));	
}

void ShipRenderer :: _deleteShip(const CoordinateFrame * cframe)
{
	for(std::vector<CoordinateModel>::iterator it = m_ships.begin();
		it != m_ships.end();
		++it)
	{
		if(it->coordframe == cframe)
		{
			m_ships.erase(it);
			return;
		}
	}
	// Might get called twice cuz we do this on both Enemy_Destroy and Enemy_Despawn
	// Not the right way to do it, but it's past midnight and we got a demo tomorrow
	//assert(0);
}