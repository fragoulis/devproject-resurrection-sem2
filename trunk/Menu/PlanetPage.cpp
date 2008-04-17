#include "PlanetPage.h"

void PlanetPage::init(int screenWidth, int screenHeight) {

	MenuItem *backButton = new MenuItem();
	backButton->init((float) screenWidth/2 - 128, 0, 256, 100, "menuExitPause.bmp", "menuExitPauseSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(backButton);


	m_backgroundImage = TextureIO::instance()->getTexture("planetMenuBg.bmp");
	m_hasBackground = true;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void PlanetPage::setItemsNumber(int itemsNumber) {
	float width = (float) m_screenWidth;
	float height = (float) m_screenHeight;
	float startX = width/5;
	float startY = height/2;
	float incrementX = 250;
	float incrementY = 200;

	int selectedItem = 0;
	MenuItem *planet1Button;
	for (int i = 0; i < itemsNumber; i++) {
		planet1Button = new MenuItem();
		if (i < itemsNumber/2)
			planet1Button->init(startX+i*incrementX, height - startY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", i == selectedItem ? MenuItem::ITEM_STATE_SELECTED : MenuItem::ITEM_STATE_UNSELECTED);
		else
			planet1Button->init(startX+(i-itemsNumber/2)*incrementX, height - startY - incrementY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", i == selectedItem ? MenuItem::ITEM_STATE_SELECTED : MenuItem::ITEM_STATE_UNSELECTED);
		m_items.push_back(planet1Button);
		planet1Button = NULL;
	}

	/*MenuItem *planet2Button = new MenuItem();
	planet2Button->init(startX+incrementX, height - startY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet2Button);

	MenuItem *planet3Button = new MenuItem();
	planet3Button->init(startX+2*incrementX, height - startY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet3Button);

	MenuItem *planet4Button = new MenuItem();
	planet4Button->init(startX, height - startY - incrementY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet4Button);

	MenuItem *planet5Button = new MenuItem();
	planet5Button->init(startX+incrementX, height - startY - incrementY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet5Button);

	MenuItem *planet6Button = new MenuItem();
	planet6Button->init(startX+2*incrementX, height - startY - incrementY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet6Button);*/
}

void PlanetPage::update(float dt) {

}




