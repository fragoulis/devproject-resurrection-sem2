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


	//m_menuPool[MENU_STATE_MAIN] = new MainPage();
	//m_menuPool[MENU_STATE_TUTORIAL] = new InstructionPage();
	//m_menuPool[MENU_STATE_CREDITS] = new CreditsPage();
	//m_menuPool[MENU_STATE_PLANET] = new PlanetPage();

	m_menuPool[MENU_STATE_MAIN] = 0;
	m_menuPool[MENU_STATE_TUTORIAL] = 0;
	m_menuPool[MENU_STATE_CREDITS] = 0;
	m_menuPool[MENU_STATE_PLANET] = 0;

	setState(MENU_STATE_MAIN);

	int screenWidth = 640;
	int screenHeight = 480;

	//for (int i = 0; i < NUM_MENU_STATES; i++) {
	//	m_menuPool[i]->init(screenWidth, screenHeight);
	//}

}

MenuRenderer::~MenuRenderer()
{
	for (int i = 0; i < NUM_MENU_STATES; i++)
		delete m_menuPool[i];
}
static MenuPage* createMenu(MenuRenderer::MenuState state)
{
	switch (state)
	{
	case MenuRenderer::MENU_STATE_MAIN : return new MainPage();
	case MenuRenderer::MENU_STATE_TUTORIAL : return new InstructionPage();
	case MenuRenderer::MENU_STATE_CREDITS : return new CreditsPage();
	case MenuRenderer::MENU_STATE_PLANET : return new PlanetPage();
	default: break;
	}
	return 0;
}

void MenuRenderer::setState(MenuState state)
{
	m_state = state;
	if (m_menuPool[state] == 0)
	{
		MenuPage* menu = createMenu(state);
		menu->init(640, 480);
		m_menuPool[state] = menu; 
	}
	m_currentMenu = m_menuPool[(int) m_state];
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

