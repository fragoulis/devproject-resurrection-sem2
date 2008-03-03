#pragma once
#include "../../utility/Singleton.h"
#include "LaserTypes.h"
#include <vector>
class ParserSection;
class Laser;

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
