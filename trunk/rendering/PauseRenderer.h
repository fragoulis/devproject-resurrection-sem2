#pragma once
#include "IRenderer.h"
#include "../Menu/MenuPage.h"

class PauseRenderer : public IRenderer
{
private:
	MenuPage *m_currentMenu;

public:
	PauseRenderer();
	virtual ~PauseRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	const int getSelectedItem() const { return m_currentMenu->getSelectedItem(); };

	void selectNextItem() { m_currentMenu->selectNextItem(); };
	void selectPreviousItem() { m_currentMenu->selectPreviousItem(); };

};
