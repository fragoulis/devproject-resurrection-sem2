#include "PausePage.h"

void PausePage::init(int screenWidth, int screenHeight) {
	MenuItem *resumeButton = new MenuItem();
	resumeButton->init(screenWidth/2.0f-128, 400, 256, 100, "menuResume.bmp", "menuResumeSel.bmp", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(resumeButton);

	MenuItem *exitButton = new MenuItem();
	exitButton->init(screenWidth/2.0f-128, 300, 256, 100, "menuExitPause.bmp", "menuExitPauseSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(exitButton);

	m_hasBackground = false;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void PausePage::update(float dt) {

}