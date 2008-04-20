#include "PausePage.h"

void PausePage::init(int screenWidth, int screenHeight) {
	MenuItem *resumeButton = new MenuItem();
	resumeButton->init(screenWidth/2.0f-128, 450, 256, 100, "menuResume.bmp", "menuResumeSel.bmp", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(resumeButton);

	MenuItem *controlsButton = new MenuItem();
	controlsButton->init(screenWidth/2.0f-128, 350, 256, 100, "menuControls.bmp", "menuControlsSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(controlsButton);

	MenuItem *exitButton = new MenuItem();
	exitButton->init(screenWidth/2.0f-128, 250, 256, 100, "menuExitPause.bmp", "menuExitPauseSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(exitButton);

	m_hasBackground = false;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void PausePage::update(float dt) {

}