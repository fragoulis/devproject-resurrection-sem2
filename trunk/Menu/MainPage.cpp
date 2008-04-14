#include "MainPage.h"

void MainPage::init(int screenWidth, int screenHeight) {
	MenuItem *startButton = new MenuItem();
	startButton->init(screenWidth/2-256, 300, 256, 100, "menuPlay.bmp", "menuPlaySel.bmp", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(startButton);

	MenuItem *creditsButton = new MenuItem();
	creditsButton->init(screenWidth/2-256, 200, 256, 100, "menuCredits.bmp", "menuCreditsSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(creditsButton);

	MenuItem *exitButton = new MenuItem();
	exitButton->init(screenWidth/2-256, 100, 256, 100, "menuExit.bmp", "menuExitSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(exitButton);


	m_backgroundImage = TextureIO::instance()->getTexture("menuBg.bmp");
	m_hasBackground = true;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void MainPage::update(float dt) {

}