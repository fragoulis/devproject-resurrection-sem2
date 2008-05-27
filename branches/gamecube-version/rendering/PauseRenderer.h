#ifndef _RES_PAUSE_RENDERER_H_
#define _RES_PAUSE_RENDERER_H_
#include "IRenderer.h"
#include "../Menu/MenuPage.h"

class PauseRenderer : public IRenderer
{
public:
	enum MenuState {
			MENU_STATE_PAUSE,
			MENU_STATE_INSTRUCTIONS,
			NUM_MENU_STATES
		};

private:
	MenuPage *m_currentMenu; //pointer to the current menu
	MenuPage *m_menuPool[NUM_MENU_STATES];
	MenuState m_state;

public:
	PauseRenderer();
	virtual ~PauseRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	const MenuState getState() const { return m_state; };
	void setState(MenuState state) { m_state = state; };

	const int getSelectedItem() const { return m_currentMenu->getSelectedItem(); };
	void setSelectedItem(int itemNumber) { m_currentMenu->setSelectedItem(itemNumber); };

	void selectNextItem() { m_currentMenu->selectNextItem(); };
	void selectPreviousItem() { m_currentMenu->selectPreviousItem(); };

};


#endif