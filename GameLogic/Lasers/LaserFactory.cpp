//*****************************************************************************
//**
//**   File:               LaserFactory.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "LaserFactory.h"
#include "Laser.h"
#include "../../utility/deleters.h"
#include "../../gfxutils/ConfParser/ParserSection.h"


LaserFactory :: LaserFactory()
{
}

LaserFactory :: ~LaserFactory()
{
	onApplicationUnload();
}

void LaserFactory :: onApplicationLoad(const ParserSection& ps)
{
	m_laserPrototypes.resize(2);
	m_laserPrototypes[PLAYER_POSITIVE] = new Laser();
	m_laserPrototypes[PLAYER_NEGATIVE] = new Laser();
	m_laserPrototypes[PLAYER_POSITIVE]->loadSettings(*ps.getSection("Laser:PlayerPositive"));
	m_laserPrototypes[PLAYER_NEGATIVE]->loadSettings(*ps.getSection("Laser:PlayerNegative"));
}

void LaserFactory :: onApplicationUnload()
{
	deleteVector(m_laserPrototypes);
}

Laser* LaserFactory :: createLaser( LaserType type ) const
{
	return new Laser(*m_laserPrototypes[type]);
}

