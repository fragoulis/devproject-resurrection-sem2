#include "MainPage.h"

void MainPage::init(int screenWidth, int screenHeight)
{
    const float posX = screenWidth * 0.5f - 450.0f;
    const float startPosY = 350.0f, deltaY = 65.0f;
    const int width  = 256;
    const int height = 64;

	MenuItem *startButton = new MenuItem();
	startButton->init(posX - 55, startPosY, width, height, "menuPlay.dds", "menuPlaySel.dds", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(startButton);

	MenuItem *tutorialButton = new MenuItem();
	tutorialButton->init(posX, startPosY-deltaY, width, height, "menuTutorial.dds", "menuTutorialSel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(tutorialButton);

	MenuItem *creditsButton = new MenuItem();
	creditsButton->init(posX - 20, startPosY-2.0f*deltaY, width, height, "menuCredits.dds", "menuCreditsSel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(creditsButton);

	MenuItem *exitButton = new MenuItem();
	exitButton->init(posX - 65, startPosY-3.0f*deltaY, width, height, "menuExit.dds", "menuExitSel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(exitButton);

	MenuItem *shipIcon = new MenuItem();
	shipIcon->init(screenWidth-750, -100, 800, 600, "enforcer.dds", "enforcer.dds", MenuItem::ITEM_STATE_UNSELECTED);
	shipIcon->setSelectable(false);
	m_items.push_back(shipIcon);


	m_backgroundImage = TextureIO::instance()->getTexture("menuBg.dds");
	m_hasBackground = true;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}
