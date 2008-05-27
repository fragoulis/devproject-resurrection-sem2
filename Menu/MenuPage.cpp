#include "MenuPage.h"
#include "../rendering/RenderEngine.h"
#include "../utility/deleters.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"
#include "../gfxutils/VA/VATTable.h"
#include "../gfx/Camera.h"
using namespace std;

MenuPage::MenuPage()
{
	m_currentTime = 0.0f;
	m_interference = 0.2f;
}

MenuPage::~MenuPage()
{
	deleteVector(m_items);
}

void MenuPage::render(Graphics &g) const
{
	Camera::activate2D();
	GXSetZMode(FALSE, GX_ALWAYS, FALSE);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetVtxDescv(VATTable::getVDL(1));

	if (m_hasBackground) {
		m_backgroundImage->bind();
		RenderEngine::drawTexturedRectangle(0, 0, 640, 480);
	}

	// Set texture alpha channel to texture red channel
	RenderEngine::useColorChannelForAlpha(GX_CH_RED);

	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	//draw items
	for (int i = 0; i < int(m_items.size()); i++) 
		m_items[i]->render(g);


	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetZMode(TRUE, GX_LEQUAL, TRUE);




	//glDisable(GL_DEPTH_TEST);
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//	glLoadIdentity();
	//	gluOrtho2D(0, m_screenWidth, 0, m_screenHeight);
	//glMatrixMode(GL_MODELVIEW);
	////here goes all the 2D rendering
	//glPushMatrix();
	//	glLoadIdentity();

	//if (m_hasBackground) {
	//}

	////draw items
	//for (int i = 0; i < (int) m_items.size(); i++) 
	//	m_items[i]->render(g);


	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
	//glMatrixMode(GL_MODELVIEW);
	////glEnable(GL_LIGHTING);
}

const int MenuPage::getSelectedItem() const {
	for (int i = 0; i < (int) m_items.size(); i++) {
		if(m_items[i]->getState()==MenuItem::ITEM_STATE_SELECTED)
			return i;
	}

	return -1;
}

void MenuPage::setSelectedItem(int itemNumber) {
	//prevent overflows
	if (itemNumber > (int) m_items.size()-1)
		return;

	//first deselect all the items
	for (int i = 0; i < (int) m_items.size(); i++) {
		m_items[i]->setState(MenuItem::ITEM_STATE_UNSELECTED);
	}
	//then select the new one
	m_items[itemNumber]->setState(MenuItem::ITEM_STATE_SELECTED);
}

bool  MenuPage::selectNextItem() {

	if (m_items.size() == 0)
		return false;

	int selectedItem = getSelectedItem();
	if (selectedItem < (int) m_items.size()-1) 
		setSelectedItem(selectedItem+1);
	else
		setSelectedItem(0);

	if (!m_items[getSelectedItem()]->isSelectable())
		selectNextItem();

	return true;
}

bool  MenuPage::selectPreviousItem() {

	if (m_items.size() == 0)
		return false;

	int selectedItem = getSelectedItem();
	if (selectedItem > 0) 
		setSelectedItem(selectedItem-1);
	else
		setSelectedItem((int) m_items.size()-1);
	
	if (!m_items[getSelectedItem()]->isSelectable())
		selectPreviousItem();

	return true;
}

void MenuPage::update(float dt) {

	m_currentTime += dt;

	for (int i = 0; i < (int) m_items.size(); i++) {
		m_items[i]->update(dt);
	}

}