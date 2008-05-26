#include "MenuRenderer.h"
#include "RenderEngine.h"
#include "../Menu/MainPage.h"
#include "../Menu/InstructionPage.h"
#include "../Menu/CreditsPage.h"
#include "../Menu/PlanetPage.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"
#include "../gfxutils/VA/VATTable.h"
#include "../gfx/Camera.h"






MenuRenderer::MenuRenderer()
{
	//int viewPortDims[4];
	//RenderEngine::instance().getViewport(viewPortDims);
	//int screenWidth = viewPortDims[2];
	//int screenHeight = viewPortDims[3];

	m_state = MENU_STATE_MAIN;

	m_menuPool[MENU_STATE_MAIN] = new MainPage();
	m_menuPool[MENU_STATE_TUTORIAL] = new InstructionPage();
	m_menuPool[MENU_STATE_CREDITS] = new CreditsPage();
	m_menuPool[MENU_STATE_PLANET] = new PlanetPage();

	int screenWidth = 640;
	int screenHeight = 480;

	for (int i = 0; i < NUM_MENU_STATES; i++) {
		m_menuPool[i]->init(screenWidth, screenHeight);

		//if (static_cast<PlanetPage*>(m_menuPool[i]))
		//	m_menuPool[i]->setItemsNumber(6);
	}

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

	m_currentMenu->update(dt);

	if (m_state == MENU_STATE_PLANET) {
		if (m_currentMenu->getSelectedItem() == 4)
			m_currentMenu->setDifficultyToShow(PlanetPage::DIFFICULTY_EASY);
		else if (m_currentMenu->getSelectedItem() == 5)
			m_currentMenu->setDifficultyToShow(PlanetPage::DIFFICULTY_MEDIUM);
		else if (m_currentMenu->getSelectedItem() == 6)
			m_currentMenu->setDifficultyToShow(PlanetPage::DIFFICULTY_MEDIUM);
		else if (m_currentMenu->getSelectedItem() == 7)
			m_currentMenu->setDifficultyToShow(PlanetPage::DIFFICULTY_HARD);
		else
			m_currentMenu->setDifficultyToShow(PlanetPage::DIFFICULTY_NONE);
	}
}

