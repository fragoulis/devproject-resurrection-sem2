//*****************************************************************************
//**
//**   File:               WorldRenderer.cpp
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-19 08:59:44 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 189 $
//**
//*****************************************************************************

#include "WorldRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Camera.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include <string>
#include <vector>

#include "../Math/Matrix44.h"

using namespace std;

WorldRenderer :: WorldRenderer()
:m_camera(0),
m_playerActive(false),
m_transpSurface(0)
{
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Despawned >(this);

	m_camera = new Camera();
	m_camera->setPerspective(30, 1.0f, 10.0f, 9000.0f);
	m_camera->setPosition(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0,0,-1));
	m_terrainRenderer.setCamera(m_camera);

	m_realCam = new Camera();
	m_realCam->setPerspective(30, 1.0f, 10.0f, 9000.0f);

	// initialize transparent surface
	vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel(0,0));
	// FIXME : Get real window extents
	int vp[4];
	RenderEngine::instance().getViewport(vp);
	m_transpSurface = new Texture2D(vp[2],vp[3],GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"transp fbo",false,false);
	m_transpFBO.Bind();
	m_transpFBO.AttachTexture(GL_TEXTURE_2D,m_transpSurface->getId(),GL_COLOR_ATTACHMENT0_EXT);
	m_transpFBO.IsValid();
	FramebufferObject::Disable();

	m_terrainRenderer.setShipRendererRef(&m_shipRenderer);
	m_terrainRenderer.setLaserRendererRef(&m_laserRenderer);
}

WorldRenderer :: ~WorldRenderer()
{
	if(m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}
	delete m_realCam;
	if(m_transpSurface)
	{
		delete m_transpSurface;
		m_transpSurface = 0;
	}
}


void WorldRenderer :: render(Graphics& g) const
{
	// Draw transparent 2D stuff in FBO
	/*
	m_transpFBO.Bind();
	glClear(GL_COLOR_BUFFER_BIT);

	m_laserRenderer.render(g);
	m_psRenderer.render(g);

	FramebufferObject::Disable();
	*/

	// now render opaque 3D stuff

	m_shipRenderer.render(g);
	m_terrainRenderer.render(g);
	m_spawnPointRenderer.render(g);
	m_laserRenderer.render(g);
	m_psRenderer.render(g);

	m_hudRenderer.render(g);

	// now overlay the quad
/*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,1,0,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);

	ShaderManager::instance()->begin("blitShader");
	m_transpSurface->bind();
	ShaderManager::instance()->setUniform1i("tex",0);
	RenderEngine::drawTexturedQuad(Vector3(0.0f,0.0f,0.0f),
								   Vector3(1.0f,0.0f,0.0f),
								   Vector3(0.0f,1.0f,0.0f),
								   Vector2(0.0f,0.0f),
								   Vector2(1.0f,1.0f));

	glDisable(GL_BLEND);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);	
*/
}

void WorldRenderer :: update( float dt )
{	
	if(m_playerActive)
	{
		const Vector3 playerpos(m_playerCoordFrame->getOrigin().cfp());
		m_camera->setPosition(playerpos + Vector3(0.0f,RenderEngine::instance().getCameraHeightAbovePlane(),0.0f),
							  playerpos,
							  Vector3(0.0f,0.0f,-1.0f));
		glLoadIdentity();
		
		m_realCam->setPosition(playerpos + Vector3(0.0f,RenderEngine::instance().getCameraHeightAbovePlane(),0.0f),
							  playerpos,
							  Vector3(0.0f,0.0f,-1.0f));
		m_realCam->slide(0.0f,-tanf(30.0f*math_const<float>::DEG2RAD)*RenderEngine::instance().getCameraHeightAbovePlane(),0.0f);
		m_realCam->pitch(30.0f);
		
		_updateMatrices();
		RenderEngine::instance().computeWsScreenEdges();
		
	}

	m_camera->update(dt);
	m_psRenderer.update(dt);
	m_spawnPointRenderer.update(dt);
	m_terrainRenderer.update(dt);
	m_hudRenderer.update(dt);
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

void WorldRenderer :: onEvent(Player_Despawned&)
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