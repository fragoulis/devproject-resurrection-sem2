//*****************************************************************************
//**
//**   File:               MenuRenderer.cpp
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "MenuRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include "../Menu/MainPage.h"
#include "../Menu/CreditsPage.h"
#include "../Menu/PlanetPage.h"
#include "gl/glu.h"
#include <gl/glee.h>





MenuRenderer::MenuRenderer()
{
	/*Texture *tex = TextureIO::instance()->getTexture("menuBg.bmp");
	m_textureList.push_back(tex);

	m_currentMenu.init(0, 0, 100, 100, "flare0.bmp", "particle2.bmp", MenuItem::ITEM_STATE_UNSELECTED);*/

	int viewPortDims[4];
	RenderEngine::instance().getViewport(viewPortDims);
	int screenWidth = viewPortDims[2];
	int screenHeight = viewPortDims[3];

	m_state = MENU_STATE_MAIN;

	m_menuPool[MENU_STATE_MAIN] = new MainPage();
	m_menuPool[MENU_STATE_CREDITS] = new CreditsPage();
	m_menuPool[MENU_STATE_PLANET] = new MainPage();

	for (int i = 0; i < NUM_MENU_STATES; i++)
		m_menuPool[i]->init(screenWidth, screenHeight);

	m_currentMenu = m_menuPool[(int) m_state];
}

MenuRenderer::~MenuRenderer()
{
	for (int i = 0; i < NUM_MENU_STATES; i++)
		delete m_menuPool[i];
}

void MenuRenderer :: render(Graphics& g) const
{
	m_currentMenu->render(g);
}

void MenuRenderer :: update(float dt)
{
	m_currentMenu = m_menuPool[(int) m_state];
}
