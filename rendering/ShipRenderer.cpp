//*****************************************************************************
//**
//**   File:               ShipRenderer.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "ShipRenderer.h"
#include "../Math/Matrix44.h"
#include "../Math/CoordinateFrame.h"
#include "../gfx/Model/Model.h"
#include "../gfx/Model/ModelMgr.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Objects/Playership.h"

#include <iostream>

using namespace std;

static char texstr[8] = "texmap0";

ShipRenderer :: ShipRenderer()
{
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Enemy_Destroyed >(this);
}

void ShipRenderer :: render(Graphics& g) const
{
	// FIXME : Do we need to do a special setup for ship rendering?
	// Maybe they should be drawn in an FBO or sth, we'll see

	// FIXME : ShaderManager->end() should be avoided. Fix that later in ALL renderers
	for(vector<CoordinateModel>::const_iterator it = m_ships.begin();
		it != m_ships.end();
		++it)
	{
		glPushMatrix();
		glMultMatrixf(it->coordframe->getMatrix().cfp());
		const float * m = it->coordframe->getMatrix().cfp();
		// Set Shader
		//ShaderManager::instance()->begin(matg.getShaderIndex());
		ShaderManager::instance()->begin("PerPixelNoTex");
		for(size_t i=0;i<it->model->getMatGroup().size();++i)
		{
			const MaterialGroup& matg = it->model->getMatGroup()[i];
			// Set Material
			matg.getMaterial().Apply();
			// Set Textures as texmap0..texmapk
			const vector<Texture *>& texlist = matg.getTextureList();
			for(size_t i=0;i<texlist.size();++i)
			{
				texstr[6] = '0' + char(i);
				texlist[i]->bind();
				ShaderManager::instance()->setUniform1i(texstr,GLint(i));
			}
			// Call VBO
			matg.getVboDesc().call();
		}
		glPopMatrix();
		ShaderManager::instance()->end();
	}
}

void ShipRenderer :: onEvent(Player_Spawned& evt)
{
	// Fetch the playership & place accordingly
	//FIXME : For now I'll just place everything straight ahead

	CoordinateModel cm(ModelMgr::instance().getModel("tri_cruiser4.obj"),&(evt.getPointer()->getCoordinateFrame()));
	m_ships.push_back(cm);
}

void ShipRenderer :: onEvent(Enemy_Spawned& evt)
{
	1;
	// Fetch the enemy & place accordingly
	// It's working fine!
	// First enemy spawns after 10 seconds
}

void ShipRenderer :: onEvent(Enemy_Destroyed& evt)
{
	// Fetch the enemy & remove
}

void ShipRenderer :: onEvent(Player_Destroyed&)
{
	// Fetch the player & remove
}