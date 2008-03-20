//*****************************************************************************
//**
//**   File:               LaserFactory.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-11 21:50:00 +0000 (Tue, 11 Mar 2008) $
//**   Last Revision:      $Revision: 142 $
//**
//*****************************************************************************


#pragma once
#include "../../utility/Singleton.h"
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

	Laser* createLaser(int type) const;


private:
	typedef std::vector<Laser*> LaserVector;
	LaserVector m_laserPrototypes;


	friend Singleton< LaserFactory >;
	LaserFactory();
	virtual ~LaserFactory();
};
