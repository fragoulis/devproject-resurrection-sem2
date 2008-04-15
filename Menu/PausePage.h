#pragma once

#include "MenuPage.h"

class PausePage: public MenuPage {
public:
	void init(int screenWidth, int screenHeight);
	void update(float dt);

};