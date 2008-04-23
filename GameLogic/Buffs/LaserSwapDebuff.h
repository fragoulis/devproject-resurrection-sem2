//*****************************************************************************
//**
//**   File:               LaserSwapDebuff.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

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
