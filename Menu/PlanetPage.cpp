#include "PlanetPage.h"

void PlanetPage::init(int screenWidth, int screenHeight) {

	MenuItem *backButton = new MenuItem();
	backButton->init(10, 0, 256, 100, "menuExitPause.bmp", "menuExitPauseSel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(backButton);

	MenuItem *difficultyLabelEasy = new MenuItem();
	difficultyLabelEasy->init((float) screenWidth/2 - 128, 100, 256, 100, "menuEasy.bmp", "menuEasy.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelEasy->setSelectable(false),
	m_items.push_back(difficultyLabelEasy);

	MenuItem *difficultyLabelMedium = new MenuItem();
	difficultyLabelMedium->init((float) screenWidth/2 - 128, 100, 256, 100, "menuMedium.bmp", "menuMedium.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelMedium->setSelectable(false);
	difficultyLabelMedium->setVisible(false);
	m_items.push_back(difficultyLabelMedium);

	MenuItem *difficultyLabelHard = new MenuItem();
	difficultyLabelHard->init((float) screenWidth/2 - 128, 100, 256, 100, "menuHard.bmp", "menuHard.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelHard->setSelectable(false);
	difficultyLabelHard->setVisible(false);
	m_items.push_back(difficultyLabelHard);


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

	//int selectedItem = 0;
	//MenuItem *planet1Button;
	//for (int i = 0; i < itemsNumber; i++) {
	//	planet1Button = new MenuItem();
	//	if (i < itemsNumber/2)
	//		planet1Button->init(startX+i*incrementX, height - startY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", i == selectedItem ? MenuItem::ITEM_STATE_SELECTED : MenuItem::ITEM_STATE_UNSELECTED);
	//	else
	//		planet1Button->init(startX+(i-itemsNumber/2)*incrementX, height - startY - incrementY, 128, 128, "planetItem.bmp", "planetItemSel.bmp", i == selectedItem ? MenuItem::ITEM_STATE_SELECTED : MenuItem::ITEM_STATE_UNSELECTED);
	//	m_items.push_back(planet1Button);
	//	planet1Button = NULL;
	//}

	MenuItem *planet2Button = new MenuItem();
	planet2Button->init(100, 500, 28, 28, "planet1_uns.bmp", "planet1_sel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet2Button);

	MenuItem *planet3Button = new MenuItem();
	planet3Button->init(700, 100, 128, 128, "planet2_uns.bmp", "planet2_sel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet3Button);

	MenuItem *planet4Button = new MenuItem();
	planet4Button->init(235, 255, 165, 165, "planet3_uns.bmp", "planet3_sel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet4Button);

	MenuItem *planet5Button = new MenuItem();
	planet5Button->init(655, 357, 50, 50, "planet4_uns.bmp", "planet4_sel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet5Button);

	//MenuItem *planet6Button = new MenuItem();
	//planet6Button->init(startX+2*incrementX, height - startY - incrementY, 128, 128, "planet1_uns.bmp", "planet1_sel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	//m_items.push_back(planet6Button);

	setSelectedItem(4);
}

void PlanetPage::update(float dt) {

}

void PlanetPage::setDifficultyToShow(int difficulty) {
	switch (difficulty) {
	case DIFFICULTY_EASY:
		m_items[1]->setVisible(true);
		m_items[2]->setVisible(false);
		m_items[3]->setVisible(false);
		break;
	case DIFFICULTY_MEDIUM:
		m_items[1]->setVisible(false);
		m_items[2]->setVisible(true);
		m_items[3]->setVisible(false);
		break;
	case DIFFICULTY_HARD:
		m_items[1]->setVisible(false);
		m_items[2]->setVisible(false);
		m_items[3]->setVisible(true);
		break;
	}
}




