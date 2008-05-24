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

	//// FIXME : ShaderManager->end() should be avoided. Fix that later in ALL renderers
	//for(vector<CoordinateModel>::const_iterator it = m_ships.begin();
	//	it != m_ships.end();
	//	++it)
	//{
	//	CHECK_GL_ERROR();
	//	glPushMatrix();
	//	glMultMatrixf(it->coordframe->getMatrix().cfp());
	//	const float * m = it->coordframe->getMatrix().cfp();
	//	// Set Shader
	//	ShaderManager::instance()->begin(it->model->getMatGroup()[0].getShaderIndex());
	//	ShaderManager::instance()->setUniform4fv("lightPosition",ldir.cfp());
	//	ShaderManager::instance()->setUniform4fv("lightColor",lcol.cfp());
	//	for(size_t i=0;i<it->model->getMatGroup().size();++i)
	//	{
	//		const MaterialGroup& matg = it->model->getMatGroup()[i];
	//		// Set Material
	//		matg.getMaterial().Apply();
	//		// Set Textures as texmap0..texmapk
	//		const vector<Texture *>& texlist = matg.getTextureList();
	//		for(size_t i=0;i<texlist.size();++i)
	//		{
	//			texstr[6] = '0' + char(i);
	//			texlist[i]->bind();
	//			ShaderManager::instance()->setUniform1i(texstr,GLint(i));
	//		}
	//		// Call VBO
	//		matg.getVboDesc().call();
	//	}
	//	glPopMatrix();
	//}
}

void ShipRenderer :: onEvent(Player_Spawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
}

void ShipRenderer :: onEvent(Player_Respawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
	// Throw an effect - ps here
}

void ShipRenderer :: onEvent(Player_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_deleteShip(cf);
}

void ShipRenderer :: onEvent(Player_Destroyed& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	//const CoordinateFrame * cf = &(evt.getValue1()->getCoordinateFrame());
	//_deleteShip(cf);
}

void ShipRenderer :: onEvent(Enemy_Spawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
	//CKLOG(string("Active ships : ") + ToString<unsigned>(unsigned(m_ships.size())), 2);
}

void ShipRenderer :: onEvent(Enemy_Despawned& evt)
{
	//CKLOG(string("Despawning ") + ToString<Enemyship*>(evt.getValue()), 3);
	//// Fetch the enemy & remove
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_deleteShip(cf);
}

void ShipRenderer :: onEvent(Enemy_Destroyed& evt)
{
	//CKLOG(string("Despawning ") + ToString<Enemyship*>(evt.getValue()), 3);
	//// Fetch the enemy & remove
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_deleteShip(cf);
}

void ShipRenderer :: onEvent(Ebomb_Spawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//_insertShip(settings,&(evt.getValue()->getCoordinateFrame()));
}

void ShipRenderer :: onEvent(Ebomb_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_deleteShip(cf);
}

void ShipRenderer :: _insertShip(const EntitySettings_t& settings, const CoordinateFrame * cframe)
{
	//Model * model = ModelMgr::instance().getModel(settings.modelName);
	//const VBO * vbo = model->getVBO();
	//const int shaderIndex =settings.shaderIndex;

	//// sort & place appropriately the model
	//// sort order = VBO -> Shader -> Texture

	//int foundpos = -1;
	//for(unsigned j=0;j<m_ships.size();++j)
	//{
	//	// check if we've found the same VBO
	//	if(vbo == m_ships[j].model->getVBO())
	//	{
	//		// now iterate through all of the ships with the same vbo, 
	//		// to find the same shader ( actually if the object has textures or not)
	//		unsigned k = j;
	//		for(; (vbo == m_ships[k].model->getVBO()) && (k < m_ships.size()); ++k)
	//		{
	//			int si = m_ships[k].model->getMatGroup()[0].getShaderIndex();
	//			if(si == shaderIndex)
	//				break;
	//		}
	//		foundpos = k;
	//	}
	//	if(foundpos != -1)
	//		break;
	//}
	//if(foundpos == -1)
	//	foundpos = int(m_ships.size());
	//m_ships.insert(m_ships.begin() + foundpos,CoordinateModel(model,cframe));
}

void ShipRenderer :: _deleteShip(const CoordinateFrame * cframe)
{
	//for(std::vector<CoordinateModel>::iterator it = m_ships.begin();
	//	it != m_ships.end();
	//	++it)
	//{
	//	if(it->coordframe == cframe)
	//	{
	//		m_ships.erase(it);
	//		return;
	//	}
	//}
	// Might get called twice cuz we do this on both Enemy_Destroy and Enemy_Despawn
	// Not the right way to do it, but it's past midnight and we got a demo tomorrow
	//assert(0);
}