//*****************************************************************************
//**
//**   File:               SpeedReductionDebuff.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


#include "SpeedReductionDebuff.h"
#include "../Spaceship.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"

SpeedReductionDebuff :: SpeedReductionDebuff(int type) : Buff(type)
{
}

SpeedReductionDebuff :: ~SpeedReductionDebuff()
{
}

Buff* SpeedReductionDebuff :: clone()
{
	return new SpeedReductionDebuff(*this);
}

void SpeedReductionDebuff :: _onActivate(WorldObject* target)
{
	Spaceship* ship = dynamic_cast<Spaceship*>(target);
	ship->multiplyThrusterFactor(1.0f / m_factor);
}

void SpeedReductionDebuff :: _onDeactivate(WorldObject* target)
{
	Spaceship* ship = dynamic_cast<Spaceship*>(target);
	ship->multiplyThrusterFactor(m_factor);
}


void SpeedReductionDebuff :: loadSettings(const ParserSection& ps)
{
	m_factor = FromString<float>(ps.getVal("Factor"));

	Buff::loadSettings(ps);
}