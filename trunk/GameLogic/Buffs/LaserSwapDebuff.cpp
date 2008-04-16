//*****************************************************************************
//**
//**   File:               LaserSwapDebuff.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

#include "LaserSwapDebuff.h"
#include "../GameLogic.h"

LaserSwapDebuff::LaserSwapDebuff(int type) : Buff(type)
{
}

LaserSwapDebuff::~LaserSwapDebuff()
{
}


Buff* LaserSwapDebuff :: clone() const
{
	return new LaserSwapDebuff(*this);
}


void LaserSwapDebuff :: _onActivate(WorldObject* target)
{
	GameLogic::instance().swapLasers();
}

void LaserSwapDebuff :: _onDeactivate(WorldObject* target)
{
	GameLogic::instance().unSwapLasers();
}
