//*****************************************************************************
//**
//**   File:               Rigidbody.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Rigidbody.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../gfxutils/Misc/utils.h"

Rigidbody :: Rigidbody()
{
	m_airdrag.factor = 1.0f;
	m_gravity.factor = 1.0f;
	m_lift.factor = 1.0f;
}

Rigidbody :: ~Rigidbody()
{
}

void Rigidbody :: setInertia( const Matrix33& i )
{
	m_inertia = i;
	m_inertiaInverse = i;
	m_inertiaInverse.invert();
}

void Rigidbody :: loadSettings( const ParserSection& ps )
{
	m_mass = FromString<float>(ps.getVal("Mass"));
	m_airdrag.coefficient = FromString<float>(ps.getVal("AirdragCoefficient"));
	m_lift.power = FromString<float>(ps.getVal("LiftPower"));
	m_lift.variance = FromString<float>(ps.getVal("LiftVariance"));
	Movable::loadSettings(ps);
}