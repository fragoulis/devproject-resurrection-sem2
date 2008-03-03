#pragma once
#include "../../utility/Singleton.h"
class ParserSection;

class LaserFactory : public Singleton< LaserFactory >
{
public:
	LaserFactory();
	virtual ~LaserFactory();

	void onApplicationLoad(const ParserSection&);
};
