#pragma once

#include "MenuPage.h"

class MainPage: public MenuPage {
public:
	~MainPage(); 

	void init(int screenWidth, int screenHeight);
	void update(float dt);
};