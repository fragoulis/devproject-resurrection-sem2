//*****************************************************************************
//**
//**   File:               LoadingRenderer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "LoadingRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include <gl/glee.h>
#include <gl/glu.h>

LoadingRenderer :: LoadingRenderer()
{
	m_texture = TextureIO::instance()->getTexture("loadingScreen.bmp");
}

LoadingRenderer :: ~LoadingRenderer()
{
}


void LoadingRenderer :: render(Graphics& g) const
{
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

	// Render loading screen
	/*glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	{
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);*/


	/*ShaderManager::instance()->begin("blitShader");
	m_texture->bind(0);
	CHECK_GL_ERROR();
	ShaderManager::instance()->setUniform1i("tex",0);
	CHECK_GL_ERROR();

	RenderEngine::drawTexturedQuad(Vector3(0, 0, 0), Vector3((float) screenWidth, 0, 0), Vector3(0, (float) screenHeight, 0), Vector2(0,0), Vector2(1,1));

	ShaderManager::instance()->end();*/


	ShaderManager::instance()->begin("blitShader");
	m_texture->bind(0);
	RenderEngine::drawTexturedQuad(Vector3(0.0f, 0.0f, 0.0f), Vector3((float) screenWidth, 0.0f, 0.0f), Vector3(0.0f, (float) screenHeight, 0.0f), Vector2(0.0f,0.0f), Vector2(1.0f,1.0f));
	ShaderManager::instance()->end();


	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_LIGHTING);
}

void LoadingRenderer :: update( float dt )
{
	// animate loading screen :)
}