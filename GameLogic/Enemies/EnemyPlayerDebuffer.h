//*****************************************************************************
//**
//**   File:               EnemyPlayerDebuffer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

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
