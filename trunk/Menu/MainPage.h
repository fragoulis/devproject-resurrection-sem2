#pragma once

#include "MenuPage.h"

class MainPage: public MenuPage {
public:
	void init(int screenWidth, int screenHeight);
	void update(float dt);

};