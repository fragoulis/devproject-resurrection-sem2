#pragma once
#include "../../utility/Singleton.h"
#include <vector>
class ParserSection;
class Laser;


/**
 * Uses a list of prototypes loaded from file to create lasers of specified type.
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
