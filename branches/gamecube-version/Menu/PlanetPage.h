#pragma once

#include "MenuPage.h"

class PlanetPage: public MenuPage {
public:
	enum {
		DIFFICULTY_EASY,
		DIFFICULTY_MEDIUM,
		DIFFICULTY_HARD,
		DIFFICULTIES_NUM,
		DIFFICULTY_NONE
	};

private:
	static const int MAX_PLANETS_NUMBER = 10;

public:
	void init(int screenWidth, int screenHeight);

	//creates the planet buttons
	void setItemsNumber(int itemsNumber);

	void setDifficultyToShow(int difficulty);
};