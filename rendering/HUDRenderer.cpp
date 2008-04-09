//*****************************************************************************
//**
//**   File:               HUDRenderer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "HUDRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include "../GameLogic/GameLogic.h"
#include "gl/glu.h"

HUDRenderer::HUDRenderer() : m_playership(0)
{
	Texture *tex = TextureIO::instance()->getTexture("flare0.bmp");
	m_textureList.push_back(tex);
}

HUDRenderer::~HUDRenderer()
{
}



void HUDRenderer :: render(Graphics& g) const
{
	// draw energy bars
	// draw number of lives
	// draw type of e-bomb. You could listen to events for e-bomb created/uncreated
	// but asking GameLogic about it is just as easy :)
	// if type is unknown, no ebomb exists
	EbombType ebombType = GameLogic::instance().getCurrentEbombType();

	int viewPortDims[4];
	RenderEngine::instance().getViewport(viewPortDims);
	int screenWidth = viewPortDims[2];
	int screenHeight = viewPortDims[3];


	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, screenWidth, 0, screenHeight);
	glMatrixMode(GL_MODELVIEW);
	//here goes all the 2D rendering
	glPushMatrix();
		glLoadIdentity();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderManager::instance()->begin("hudShader");
		m_textureList[TEXTURE_RED_BAR]->bind(0);
		CHECK_GL_ERROR();
		ShaderManager::instance()->setUniform1i("tex",0);
		CHECK_GL_ERROR();
		const GLfloat transparency = 0.5f;
		ShaderManager::instance()->setUniform1fv("transparency", &transparency);
		CHECK_GL_ERROR();

		RenderEngine::drawTexturedQuad(Vector3(0, 0, 0), Vector3(200, 0, 0), Vector3(0, 200, 0), Vector2(0,0), Vector2(1,1));

		ShaderManager::instance()->end();

		glDisable(GL_BLEND);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_LIGHTING);
}

void HUDRenderer :: update(float dt)
{
}


void HUDRenderer :: onEvent(Player_Spawned& evt)
{
	m_playership = evt.getValue();
}

void HUDRenderer :: onEvent(Player_Despawned&)
{
	m_playership = 0;
}