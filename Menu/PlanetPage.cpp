#include "PlanetPage.h"

void PlanetPage::init(int screenWidth, int screenHeight) {

	MenuItem *backButton = new MenuItem();
	backButton->init(10, 20, 256, 100, "menuExitPause.dds", "menuExitPauseSel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(backButton);

	MenuItem *difficultyLabelEasy = new MenuItem();
	difficultyLabelEasy->init((float) screenWidth/2 - 128, 100, 256, 100, "menuEasy.dds", "menuEasy.dds", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelEasy->setSelectable(false),
	m_items.push_back(difficultyLabelEasy);

	MenuItem *difficultyLabelMedium = new MenuItem();
	difficultyLabelMedium->init((float) screenWidth/2 - 128, 100, 256, 100, "menuMedium.dds", "menuMedium.dds", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelMedium->setSelectable(false);
	difficultyLabelMedium->setVisible(false);
	m_items.push_back(difficultyLabelMedium);

	MenuItem *difficultyLabelHard = new MenuItem();
	difficultyLabelHard->init((float) screenWidth/2 - 128, 100, 256, 100, "menuHard.dds", "menuHard.dds", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelHard->setSelectable(false);
	difficultyLabelHard->setVisible(false);
	m_items.push_back(difficultyLabelHard);


	m_backgroundImage = TextureIO::instance()->getTexture("planetMenuBg.bmp");
	m_hasBackground = true;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_interference = 0.1f;
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
	//		planet1Button->init(startX+i*incrementX, height - startY, 128, 128, "planetItem.dds", "planetItemSel.dds", i == selectedItem ? MenuItem::ITEM_STATE_SELECTED : MenuItem::ITEM_STATE_UNSELECTED);
	//	else
	//		planet1Button->init(startX+(i-itemsNumber/2)*incrementX, height - startY - incrementY, 128, 128, "planetItem.dds", "planetItemSel.dds", i == selectedItem ? MenuItem::ITEM_STATE_SELECTED : MenuItem::ITEM_STATE_UNSELECTED);
	//	m_items.push_back(planet1Button);
	//	planet1Button = NULL;
	//}

	MenuItem *planet2Button = new MenuItem();
	planet2Button->init(80, 480, 64, 64, "black.bmp", "planet1_sel.bmp", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet2Button);

	MenuItem *planet3Button = new MenuItem();
	planet3Button->init(700, 100, 128, 128, "black.bmp", "planet2_sel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet3Button);

	MenuItem *planet4Button = new MenuItem();
	planet4Button->init(235, 255, 165, 165, "black.bmp", "planet3_sel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet4Button);

	MenuItem *planet5Button = new MenuItem();
	planet5Button->init(645, 355, 50, 50, "black.bmp", "planet4_sel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(planet5Button);

	//MenuItem *planet6Button = new MenuItem();
	//planet6Button->init(startX+2*incrementX, height - startY - incrementY, 128, 128, "planet1_uns.dds", "planet1_sel.dds", MenuItem::ITEM_STATE_UNSELECTED);
	//m_items.push_back(planet6Button);

	setSelectedItem(4);
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
	default:
		m_items[1]->setVisible(false);
		m_items[2]->setVisible(false);
		m_items[3]->setVisible(false);
		break;
	}
}




