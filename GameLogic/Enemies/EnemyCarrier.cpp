//*****************************************************************************
//**
//**   File:               EnemyCarrier.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

#include "EnemyCarrier.h"

EnemyCarrier :: EnemyCarrier(int type) : Enemyship(type)
{
	
}


EnemyCarrier :: ~EnemyCarrier()
{
}

Enemyship* EnemyCarrier :: clone()
{
	return new EnemyCarrier(*this);
}

void EnemyCarrier :: update(float dt)
{
}

