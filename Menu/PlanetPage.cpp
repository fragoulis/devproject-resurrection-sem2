#include "PlanetPage.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"
#include "../rendering/RenderEngine.h"
class Graphics;

class PlanetItem : public MenuItem
{
public:
	PlanetItem(PlanetPage* page, int posX, int posY, int width, int height, PlanetPage::PlanetDifficulty difficulty)
	{
		m_posX = posX;
		m_posY = posY;
		m_width = width;
		m_height = height;
		m_state = MenuItem::ITEM_STATE_UNSELECTED;
		m_selectable = true;
		m_visible = true;
		m_difficulty = difficulty;
		m_page = page;
	}
	virtual ~PlanetItem() {	}
	virtual void update(float dt) {
		MenuItem::update(dt);
		if (isSelected()) m_page->setDifficultyToShow(m_difficulty); // slow but doesn't matter
	}
	virtual void render(Graphics& g) const
	{
		Texture* tex = TextureMgr::instance().getTexture("menuSelectionFrame");
		tex->bind();
		RenderEngine::useColorChannelForAlpha(GX_CH_ALPHA);
		if (m_visible && m_state == ITEM_STATE_SELECTED)
		{
			RenderEngine::drawTexturedRectangle(m_posX, m_posY, m_width, m_height);
		}
		RenderEngine::useColorChannelForAlpha(GX_CH_RED);
	}
private:
	PlanetPage* m_page;
	PlanetPage::PlanetDifficulty m_difficulty;
};



void PlanetPage::init(int screenWidth, int screenHeight)
{
	TextureMgr::safeInstance().loadPalette("planetItems.tpl", "planetItemsTPL.txt");

	MenuItem *backButton = new MenuItem();
	backButton->init(0, 0, 128, 50, "menuExitPause", "menuExitPauseSel", MenuItem::ITEM_STATE_UNSELECTED);
	m_items.push_back(backButton);

	MenuItem *difficultyLabelEasy = new MenuItem();
	difficultyLabelEasy->init(192, 0, 256, 100, "menuEasy", "menuEasy", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelEasy->setSelectable(false),
	m_items.push_back(difficultyLabelEasy);

	MenuItem *difficultyLabelMedium = new MenuItem();
	difficultyLabelMedium->init(192, 0, 256, 100, "menuMedium", "menuMedium", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelMedium->setSelectable(false);
	difficultyLabelMedium->setVisible(false);
	m_items.push_back(difficultyLabelMedium);

	MenuItem *difficultyLabelHard = new MenuItem();
	difficultyLabelHard->init(192, 0, 256, 100, "menuHard", "menuHard", MenuItem::ITEM_STATE_UNSELECTED);
	difficultyLabelHard->setSelectable(false);
	difficultyLabelHard->setVisible(false);
	m_items.push_back(difficultyLabelHard);

	m_items.push_back(new PlanetItem(this, 130, 155, 128, 128, DIFFICULTY_EASY));
	m_items.push_back(new PlanetItem(this, 59, 309, 64, 64, DIFFICULTY_EASY));
	m_items.push_back(new PlanetItem(this, 390, 205, 64, 64, DIFFICULTY_MEDIUM));
	m_items.push_back(new PlanetItem(this, 415, 70, 75, 75, DIFFICULTY_HARD));


	const std::string texname = "planetMenuBg";
	TextureMgr::safeInstance().loadPalette(texname + ".tpl", texname + "TPL.txt");
	m_backgroundImage = TextureMgr::instance().getTexture(texname);
	m_hasBackground = true;
 
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_interference = 0.1f;

	setSelectedItem(4);
}


void PlanetPage::setDifficultyToShow(PlanetDifficulty difficulty) {
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




