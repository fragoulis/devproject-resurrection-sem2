//*****************************************************************************
//**
//**   File:               Enemyship.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************


#include "Enemyship.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../Objects/Playership.h"
#include "../../utility/EventManager.h"
#include "../GameEvents.h"

Enemyship :: Enemyship(int type)
{
	m_state = ALIVE;
	setType(type);
}

Enemyship :: ~Enemyship()
{
}

void Enemyship :: update(float dt)
{
}

Enemyship* Enemyship :: clone()
{
	return new Enemyship(*this);
}


void Enemyship :: loadSettings( const ParserSection& ps )
{
	m_energyType = EnergyTypeFromString(ps.getVal("EnergyType"));
	m_collisionPower = FromString<int>(ps.getVal("CollisionPower"));
	m_hitPoints = float(FromString<int>(ps.getVal("HitPoints")));
	m_energyPoints = FromString<int>(ps.getVal("EnergyPoints"));
	Spaceship::loadSettings(ps);
}

void Enemyship :: collideWithPlayer(Playership* player)
{
	// remove enemy from the game
	setToBeDeleted();

	// check if player is invulnerable
	if (player->isInvulnerable() || player->isDying()) return;

	// get player energy, reduce it by enemy's collision power
	EnergyType type = getEnergyType();
	int enemyPower = getCollisionPower();
	int playerEnergy = player->getEnergy(type) - enemyPower;

	// check if player can sustain the damage taken
	if (playerEnergy < 0) {
		EventManager::instance().fireEvent(Player_Destroyed(player, type));
	}
	else {
		player->setEnergy(type, playerEnergy);
		EventManager::instance().fireEvent(Player_EnergyDrained(player, type, enemyPower));
	}
}