#pragma once

#include "MenuItem.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include <vector>

class Graphics;
class Texture;

using namespace std;

class MenuPage {
protected:
	vector<MenuItem*> m_items;
	Texture *m_backgroundImage;
	int m_screenWidth, m_screenHeight;
	bool m_hasBackground; //flag to check if the menu page has a background to render

	float m_currentTime;
	float m_interference;

	vector<Texture*> m_textureList;

public:
	MenuPage();
	virtual ~MenuPage(); 

	virtual void init(int screenWidth, int screenHeigth)=0;
	virtual void update(float dt);
	virtual void render(Graphics &g) const;

	virtual void setItemsNumber(int itemsNumber) {};
	virtual void setDifficultyToShow(int difficulty) {};

	const int getSelectedItem() const;
	void setSelectedItem(int itemNumber);
	bool selectNextItem();
	bool selectPreviousItem();
};