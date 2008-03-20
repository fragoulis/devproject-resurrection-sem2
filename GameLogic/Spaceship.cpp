//*****************************************************************************
//**
//**   File:               Spaceship.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#include "Spaceship.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../gfxutils/Misc/utils.h"

Spaceship :: Spaceship()
{
	m_radius = 0.0f;
	m_thrusterData.power = 0.0f;
	m_thrusterData.maxPower = 0.0f;
	m_thrusterData.factor = 1.0f;
}

Spaceship :: ~Spaceship()
{
}


void Spaceship :: loadSettings(const ParserSection& ps)
{
	m_thrusterData.maxPower = FromString<float>(ps.getVal("ThrusterMaxPower"));
	m_radius = FromString<float>(ps.getVal("Radius"));
	Rigidbody::loadSettings(ps);

	float I = 2.0f / 5.0f * getMass() * m_radius * m_radius;
	setInertia(Matrix33(I));
}