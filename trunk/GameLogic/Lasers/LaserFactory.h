//*****************************************************************************
//**
//**   File:               LaserFactory.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../../utility/Singleton.h"
#include "LaserTypes.h"
#include <vector>
class ParserSection;
class Laser;

/**
 * Uses a list of prototypes loaded from file
 * at app::load to create lasers of specified type.
 */
class LaserFactory : public Singleton< LaserFactory >
{
public:

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

	Laser* createLaser(LaserType type) const;


private:
	typedef std::vector<Laser*> LaserVector;
	LaserVector m_laserPrototypes;


	friend Singleton< LaserFactory >;
	LaserFactory();
	virtual ~LaserFactory();
};