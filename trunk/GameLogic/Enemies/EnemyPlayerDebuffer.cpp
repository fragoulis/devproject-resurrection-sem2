//*****************************************************************************
//**
//**   File:               EnemyPlayerDebuffer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

#include "EnemyPlayerDebuffer.h"
#include "../Objects/Playership.h"
#include "../Buffs/BuffFactory.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"


EnemyPlayerDebuffer :: EnemyPlayerDebuffer(int type) : Enemyship(type)
{
}

EnemyPlayerDebuffer :: ~EnemyPlayerDebuffer()
{
}

Enemyship* EnemyPlayerDebuffer :: clone()
{
	return new EnemyPlayerDebuffer(*this);
}

void EnemyPlayerDebuffer :: collideWithPlayer(Playership* player)
{
	// We delete ourselves
	setToBeDeleted();

	// check if player is invulnerable
	if (player->isInvulnerable() || player->isDying()) return;

	// add debuff. If it is already present, will add another stack and restart timer
	// Speed reduction is set to a max of 1 stack though
	player->addBuff(m_debuffType);
}

void EnemyPlayerDebuffer :: loadSettings(const ParserSection& ps)
{
	std::string debuffName = ps.getVal("Debuff");
	m_debuffType = BuffFactory::instance().getTypeFromName(debuffName);

	Enemyship::loadSettings(ps);
}