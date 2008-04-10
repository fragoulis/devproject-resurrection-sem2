//*****************************************************************************
//**
//**   File:               SpeedReductionDebuff.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

#pragma once
#include "Buff.h"
class WorldObject;

class SpeedReductionDebuff : public Buff
{
public:
	SpeedReductionDebuff(int type);
	virtual ~SpeedReductionDebuff();

	virtual void loadSettings(const ParserSection&);
	virtual Buff* clone();

private:
	float m_factor;


	virtual void _onActivate(WorldObject* target);
	virtual void _onDeactivate(WorldObject* target);
};
