#pragma once

#include "MenuPage.h"

class PlanetPage: public MenuPage {
private:
	static const int MAX_PLANETS_NUMBER = 10;

public:
	void init(int screenWidth, int screenHeight);
	void update(float dt);

	//creates the planet buttons
	void setItemsNumber(int itemsNumber);
};