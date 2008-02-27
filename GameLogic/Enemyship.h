#pragma once
#include "Spaceship.h"

class Enemyship : public Spaceship
{
public:
	Enemyship();
	virtual ~Enemyship();

	int getType() const { return m_type; }
	void setType(int t) { m_type = t; }

private:
	int m_type;
};
