#pragma once
#include "Enemyship.h"

/**
 * This type of enemy does not drain energy from the player when it collides with the player
 * It instead applies a debuff to the player
 * This is used to create the interceptors and disrupters
 */
class EnemyPlayerDebuffer : public Enemyship
{
public:
	EnemyPlayerDebuffer(int type);
	virtual ~EnemyPlayerDebuffer();

	virtual void loadSettings(const ParserSection&);
	virtual void collideWithPlayer(Playership* player);
	virtual Enemyship* clone();

private:
	int m_debuffType;
};
