#pragma once
#include "Buff.h"



/**
 * Special buff that swapped the laser mouse buttons
 *
 */
class LaserSwapDebuff : public Buff
{
public:
	LaserSwapDebuff(int type);
	virtual ~LaserSwapDebuff();

	virtual Buff* clone() const;

private:

	virtual void _onActivate(WorldObject* target);
	virtual void _onDeactivate(WorldObject* target);
};
