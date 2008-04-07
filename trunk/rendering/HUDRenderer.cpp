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
#include "../GameLogic/GameLogic.h"
#include "gl/glu.h"

HUDRenderer::HUDRenderer() : m_playership(0)
{
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

		/*glColor4f(1,0,0,1);
		RenderEngine::drawQuad(Vector3(0, 0, 0), Vector3(200, 0, 0), Vector3(0, 200, 0));*/



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