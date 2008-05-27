#ifndef _RES_PLANETPAGE_H_
#define _RES_PLANETPAGE_H_

#include "MenuPage.h"

class PlanetPage: public MenuPage {
public:
	enum PlanetDifficulty {
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

	void setDifficultyToShow(PlanetDifficulty difficulty);
};

#endif
