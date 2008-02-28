#pragma once
#include "Spaceship.h"

class Enemyship : public Spaceship
{
public:
	Enemyship();
	Enemyship(int type) : m_type(type) { }
	virtual ~Enemyship();

	int getType() const { return m_type; }
	void setType(int t) { m_type = t; }

private:
	int m_type;
};
