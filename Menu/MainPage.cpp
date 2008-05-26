#include "MainPage.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"

void MainPage::init(int screenWidth, int screenHeight)
{
    const int posX = 128;
    const int startPosY = 150, deltaY = 65;
    const int width  = 256;
    const int height = 64;

	TextureMgr::safeInstance().loadPalette("menuItems.tpl", "menuItemsTPL.txt");

	MenuItem *startButton = new MenuItem();
	startButton->init(posX /*- 55*/, startPosY, width, height, "menuPlay", "menuPlaySel", MenuItem::ITEM_STATE_SELECTED);
	m_items.push_back(startButton);

	MenuItem *tutorialButton = new MenuItem();
	tutorialButton->init(posX, startPosY-deltaY, width, height, "menuTutorial", "menuTutorialSel", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(tutorialButton);

	MenuItem *creditsButton = new MenuItem();
	creditsButton->init(posX /*- 20*/, startPosY-2*deltaY, width, height, "menuCredits", "menuCreditsSel", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(creditsButton);

	//MenuItem *exitButton = new MenuItem();
	//exitButton->init(posX /*- 65*/, startPosY-3.0f*deltaY, width, height, "menuExit", "menuExitSel", MenuItem::ITEM_STATE_UNSELECTED);
	//m_items.push_back(exitButton);

	//MenuItem *shipIcon = new MenuItem();
	//shipIcon->init(screenWidth-750, -100, 800, 600, "enforcer", "enforcer", MenuItem::ITEM_STATE_UNSELECTED);
	//shipIcon->setSelectable(false);
	//m_items.push_back(shipIcon);



	const std::string texname = "menuBG";
	TextureMgr::safeInstance().loadPalette(texname + ".tpl", texname + "TPL.txt");
	m_backgroundImage = TextureMgr::instance().getTexture(texname);
	m_hasBackground = true;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}
