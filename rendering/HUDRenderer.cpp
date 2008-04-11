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
#include "../GameLogic/Objects/Playership.h"
#include "gl/glu.h"

HUDRenderer::HUDRenderer() : m_playership(0), m_currentLives(0), m_ebombType(EBOMB_TYPE_UNKNOWN)
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

		//DRAW LIFES INFO
		for (int i = 0; i < m_currentLives; i++)
			RenderEngine::drawTexturedQuad(Vector3((float) (i*50.0f), 0, 0), Vector3(50.0f, 0, 0), Vector3(0, 50.0f, 0), Vector2(0,0), Vector2(1,1));

		//DRAW E-BOMB INFO
		switch (m_ebombType) {
			case EBOMB_TYPE_UNKNOWN:
				break;
			default:
				RenderEngine::drawTexturedQuad(Vector3(screenWidth-80.0f, 0, 0), Vector3(50.0f, 0, 0), Vector3(0, 50.0f, 0), Vector2(0,0), Vector2(1,1));
				break;

		}

		//DRAW ENERGY BARS
		if (m_playership != NULL) {
			int energyCapacity = m_playership->getEnergyCapacity();
			int redEnergyAmount = m_playership->getEnergy(ENERGY_TYPE_RED);
			int yellowEnergyAmount = m_playership->getEnergy(ENERGY_TYPE_YELLOW);
			int blueEnergyAmount = m_playership->getEnergy(ENERGY_TYPE_BLUE);
			//RED BAR
			for (int i = 0; i < redEnergyAmount; i++) {
				RenderEngine::drawTexturedQuad(Vector3((float) (i*10.0f) + (screenWidth/2.0f-(energyCapacity*10)/2), 30.0f, 0), Vector3(10.0f, 0.0f, 0), Vector3(0, 10.0f, 0), Vector2(0,0), Vector2(1,1));
			}
			//YELLOW BAR
			for (int i = 0; i < yellowEnergyAmount; i++) {
				RenderEngine::drawTexturedQuad(Vector3((float) (i*10.0f) + (screenWidth/2.0f-(energyCapacity*10)/2), 20.0f, 0), Vector3(10.0f, 0, 0), Vector3(0, 10.0f, 0), Vector2(0,0), Vector2(1,1));
			}
			//BLUE BAR
			for (int i = 0; i < blueEnergyAmount; i++) {
				RenderEngine::drawTexturedQuad(Vector3((float) (i*10.0f) + (screenWidth/2.0f-(energyCapacity*10)/2), 10.0f, 0), Vector3(10.0f, 0, 0), Vector3(0, 10.0f, 0), Vector2(0,0), Vector2(1,1));
			}
		}


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
	m_playership = GameLogic::instance().getPlayership();
	m_currentLives = GameLogic::instance().getCurrentLives();
	m_ebombType = GameLogic::instance().getCurrentEbombType();
}


void HUDRenderer :: onEvent(Player_Spawned& evt)
{
	m_playership = evt.getValue();
}

void HUDRenderer :: onEvent(Player_Despawned&)
{
	m_playership = 0;
}