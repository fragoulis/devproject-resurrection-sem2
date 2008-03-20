//*****************************************************************************
//**
//**   File:               Movable.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#include "Movable.h"

Movable :: Movable() : m_velocity(0.0f, 0.0f, 0.0f), m_angularVelocity(0.0f, 0.0f, 0.0f)
{
}

Movable :: ~Movable()
{
}
