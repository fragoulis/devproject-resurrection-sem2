//*****************************************************************************
//**
//**   File:               WorldRenderer.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "WorldRenderer.h"
#include "../gfx/Camera.h"

WorldRenderer :: WorldRenderer()
:m_camera(0),
m_playerActive(false)
{
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);

	m_camera = new Camera();
	m_camera->setPerspective(30, 1.0f, 10.0f, 9000.0f);
	m_camera->setPosition(Vector3(0, 300, -0), Vector3(0, 0, -0), Vector3(0,0,-1));
	m_terrainRenderer.setCamera(m_camera);
}

WorldRenderer :: ~WorldRenderer()
{
	if(m_camera)
		delete m_camera;
}


void WorldRenderer :: render(Graphics& g) const
{
	m_terrainRenderer.render(g);
	m_spawnPointRenderer.render(g);
	m_shipRenderer.render(g);
	m_laserRenderer.render(g);
	m_psRenderer.render(g);
}

void WorldRenderer :: update( float dt )
{
	// FIXME : looks ugly, camera should be ChaseCamera, with the object as an attachment, and
	//		   to be able to unattach it ( to stay constant)
	if(m_playerActive)
	{
		const Vector3 playerpos(m_playerCoordFrame->getOrigin().cfp());
		m_camera->setPosition(playerpos + Vector3(0.0f,1500.0f,0.0f),
							  playerpos,
							  Vector3(0.0f,0.0f,-1.0f));
	}
	m_camera->update(dt);
	_updateMatrices();
	m_psRenderer.update(dt);
	m_spawnPointRenderer.update(dt);
	m_terrainRenderer.update(dt);
	//m_shipRenderer.update(dt);
}

void WorldRenderer :: newCamera(Camera * cam)
{
	if(m_camera)
		delete m_camera;
	m_camera = cam;
}

void WorldRenderer :: onEvent(Player_Spawned& evt)
{
	m_playerActive = true;
	m_playerCoordFrame = &(evt.getValue()->getCoordinateFrame());
}

void WorldRenderer :: onEvent(Player_Destroyed&)
{
	m_playerActive = false;
}


void WorldRenderer :: _updateMatrices()
{
	glGetFloatv(GL_MODELVIEW_MATRIX,m_mviewf);
	glGetFloatv(GL_PROJECTION_MATRIX,m_projf);
	for(int i=0;i<16;++i)
	{
		m_mviewd[i] = double(m_mviewf[i]);
		m_projd[i] = double(m_projf[i]);
	}
}