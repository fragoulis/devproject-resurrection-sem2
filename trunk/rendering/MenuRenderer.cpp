//*****************************************************************************
//**
//**   File:               MenuRenderer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "MenuRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include "gl/glu.h"
#include <gl/glee.h>





MenuRenderer::MenuRenderer()
{
	/*Texture *tex = TextureIO::instance()->getTexture("menuBg.bmp");
	m_textureList.push_back(tex);

	testItem.init(0, 0, 100, 100, "flare0.bmp", "particle2.bmp", MenuItem::ITEM_STATE_UNSELECTED);*/

	int viewPortDims[4];
	RenderEngine::instance().getViewport(viewPortDims);
	int screenWidth = viewPortDims[2];
	int screenHeight = viewPortDims[3];

	testItem.init(screenWidth, screenHeight);
}

MenuRenderer::~MenuRenderer()
{
}

void MenuRenderer :: render(Graphics& g) const
{
	//int viewPortDims[4];
	//RenderEngine::instance().getViewport(viewPortDims);
	//int screenWidth = viewPortDims[2];
	//int screenHeight = viewPortDims[3];


	////glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//	glLoadIdentity();
	//	gluOrtho2D(0, screenWidth, 0, screenHeight);
	//glMatrixMode(GL_MODELVIEW);
	////here goes all the 2D rendering
	//glPushMatrix();
	//	glLoadIdentity();


	//	ShaderManager::instance()->begin("blitShader");
	//	
	//	m_textureList[0]->bind(0);
	//	RenderEngine::drawTexturedQuad(Vector3(0.0f, 0, 0), Vector3((float) screenWidth, 0, 0), Vector3(0, (float) screenHeight, 0), Vector2(0,0), Vector2(1,1));


	//	ShaderManager::instance()->end();

	//	testItem.render(g);


	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
	//glMatrixMode(GL_MODELVIEW);
	////glEnable(GL_LIGHTING);

	testItem.render(g);
}

void MenuRenderer :: update(float dt)
{
}
