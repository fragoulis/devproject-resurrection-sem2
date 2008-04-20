#include "MainPage.h"

void MainPage::init(int screenWidth, int screenHeight) {
	MenuItem *startButton = new MenuItem();
	startButton->init(screenWidth/2.0f-256, 350, 256, 100, "menuPlay.bmp", "menuPlaySel.bmp", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(startButton);

	MenuItem *tutorialButton = new MenuItem();
	tutorialButton->init(screenWidth/2.0f-256, 250, 256, 100, "menuTutorial.bmp", "menuTutorialSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(tutorialButton);

	MenuItem *creditsButton = new MenuItem();
	creditsButton->init(screenWidth/2.0f-256, 150, 256, 100, "menuCredits.bmp", "menuCreditsSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(creditsButton);

	MenuItem *exitButton = new MenuItem();
	exitButton->init(screenWidth/2.0f-256, 50, 256, 100, "menuExit.bmp", "menuExitSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(exitButton);


	m_backgroundImage = TextureIO::instance()->getTexture("menuBg.bmp");
	m_hasBackground = true;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void MainPage::update(float dt) {

}