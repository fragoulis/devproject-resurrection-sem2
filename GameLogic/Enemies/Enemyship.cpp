//*****************************************************************************
//**
//**   File:               Enemyship.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Enemyship.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"

Enemyship :: Enemyship() : m_state(ALIVE)
{
}

Enemyship :: Enemyship(int type)
{
	m_state = ALIVE;
	setType(type);
}

Enemyship :: ~Enemyship()
{
}


void Enemyship :: loadSettings( const ParserSection& ps )
{
	m_energyType = EnergyTypeFromString(ps.getVal("EnergyType"));
	m_collisionPower = FromString<int>(ps.getVal("CollisionPower"));
	m_hitpoints = FromString<int>(ps.getVal("HitPoints"));
	Spaceship::loadSettings(ps);
}