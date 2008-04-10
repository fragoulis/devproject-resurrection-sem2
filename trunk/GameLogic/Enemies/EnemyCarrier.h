//*****************************************************************************
//**
//**   File:               EnemyCarrier.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


#pragma once
#include "Enemyship.h"

class EnemyCarrier : public Enemyship
{
public:
	EnemyCarrier(int type);
	virtual ~EnemyCarrier();

	virtual void update(float dt);
	virtual Enemyship* clone();

};
