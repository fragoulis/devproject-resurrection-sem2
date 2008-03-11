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
#include "../WorldObjectTypeManager.h"
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
	int typeCount = WorldObjectTypeManager::instance().getTypeCount();
	m_laserPrototypes.resize(typeCount);

	typedef std::vector<const ParserSection *> PSVector;
	const ParserSection* psLaser = ps.getSection("Laser");
	PSVector psTypes = psLaser->getChildren();

	// loop over all sections
	// each section is a laser type
	// and the name should correspond to a world object type
	for (PSVector::iterator it = psTypes.begin(); it != psTypes.end(); it++)
	{
		const ParserSection* ps = *it;
		int type = WorldObjectTypeManager::instance().getTypeFromName(ps->getName());
		Laser* laser = new Laser(type);
		laser->loadSettings(*ps);
		m_laserPrototypes[type] = laser;
	}
}

void LaserFactory :: onApplicationUnload()
{
	deleteVector(m_laserPrototypes);
}

Laser* LaserFactory :: createLaser( int type ) const
{
	Laser* prototype = m_laserPrototypes[type];
	assert(prototype != 0);
	Laser* newlaser = new Laser(*prototype);
	return newlaser;
}

