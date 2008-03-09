//*****************************************************************************
//**
//**   File:               Spawnpoint.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../WorldObject.h"
class ParserSection;

class Spawnpoint : public WorldObject
{
public:
	Spawnpoint();
	virtual ~Spawnpoint();

	void update(float dt);

	void loadSettings(const ParserSection&);

private:
	int m_spawnType;
	int m_spawnCount;
	float m_spawnPeriodTime;
	float m_timeTillNextSpawn;
};
