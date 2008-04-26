#include "PausePage.h"

void PausePage::init(int screenWidth, int screenHeight) 
{
    const float posX = screenWidth * 0.5f - 128.0f;
    const float startPosY = 400.0f, deltaY = 65.0f;
    const int width  = 256;
    const int height = 64;

	MenuItem *resumeButton = new MenuItem();
	resumeButton->init(posX, startPosY, width, height, "menuResume.bmp", "menuResumeSel.bmp", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(resumeButton);

	MenuItem *controlsButton = new MenuItem();
	controlsButton->init(posX, startPosY-deltaY, width, height, "menuControls.bmp", "menuControlsSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(controlsButton);

	MenuItem *exitButton = new MenuItem();
	exitButton->init(posX, startPosY-2*deltaY, width, height, "menuExitPause.bmp", "menuExitPauseSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(exitButton);

	m_hasBackground = false;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void PausePage::update(float dt) {

}