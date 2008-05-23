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
