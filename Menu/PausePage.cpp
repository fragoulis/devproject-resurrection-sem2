#include "PausePage.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"

void PausePage::init(int screenWidth, int screenHeight) 
{
    const int posX = 192;
    const int startPosY = 200, deltaY = 65;
    const int width  = 256;
    const int height = 64;

	TextureMgr::safeInstance().loadPalette("pauseItems.tpl", "pauseItemsTPL.txt");

	MenuItem *resumeButton = new MenuItem();
	resumeButton->init(posX, startPosY, width, height, "menuResume", "menuResumeSel", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(resumeButton);

	//MenuItem *controlsButton = new MenuItem();
	//controlsButton->init(192, startPosY-deltaY, width, height, "menuControls", "menuControlsSel", MenuItem::ITEM_STATE_UNSELECTED);
	//m_items.push_back(controlsButton);

	MenuItem *exitButton = new MenuItem();
	exitButton->init(posX, startPosY-deltaY, width, height, "menuExitPause", "menuExitPauseSel", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(exitButton);

	m_hasBackground = false;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}
