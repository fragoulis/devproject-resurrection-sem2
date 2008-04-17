//*****************************************************************************
//**
//**   File:               MenuRenderer.h
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"
#include "../Menu/MenuPage.h"

class Texture;

using namespace std;

class MenuRenderer : public IRenderer
{
public:
	enum MenuState {
		MENU_STATE_MAIN,
		MENU_STATE_CREDITS,
		MENU_STATE_PLANET,
		NUM_MENU_STATES
	};

private:
	MenuPage *m_currentMenu; //pointer to the current menu
	MenuPage *m_menuPool[NUM_MENU_STATES];
	MenuState m_state;

public:
	MenuRenderer();
	virtual ~MenuRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	const MenuState getState() const { return m_state; };
	void setState(MenuState state) { m_state = state; };

	const int getSelectedItem() const { return m_currentMenu->getSelectedItem(); };
	void setSelectedItem(int itemNumber) { m_currentMenu->setSelectedItem(itemNumber); };

	bool selectNextItem() { if (m_currentMenu->selectNextItem()) return true; return false; };
	bool selectPreviousItem() { if (m_currentMenu->selectPreviousItem()) return true; return false; };

};
