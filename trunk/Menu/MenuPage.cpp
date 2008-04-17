#include "MenuPage.h"
#include "../rendering/RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../utility/deleters.h"
#include "gl/glu.h"
#include <gl/glee.h>

MenuPage::~MenuPage() {
	deleteVector(m_items);
	//std::vector<MenuItem *>::iterator it = m_items.begin();
	//while(it != m_items.end())
	//{
	//	MenuItem *item = *it;
	//	it = m_items.erase(it);
	//	delete item;
	//}
}

void MenuPage::render(Graphics &g) const {

	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, m_screenWidth, 0, m_screenHeight);
	glMatrixMode(GL_MODELVIEW);
	//here goes all the 2D rendering
	glPushMatrix();
		glLoadIdentity();

	//draw background
	if (m_hasBackground) {
		ShaderManager::instance()->begin("blitShader");
		m_backgroundImage->bind(0);
		RenderEngine::drawTexturedQuad(Vector3(0.0f, 0.0f, 0.0f), Vector3((float) m_screenWidth, 0.0f, 0.0f), Vector3(0.0f, (float) m_screenHeight, 0.0f), Vector2(0.0f,0.0f), Vector2(1.0f,1.0f));
		ShaderManager::instance()->end();
	}

	//draw items
	for (int i = 0; i < (int) m_items.size(); i++) 
		m_items[i]->render(g);


	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_LIGHTING);
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

	//if (!m_items[itemNumber]->isSelectable())
	//	return;

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