#pragma once
#include "WorldObject.h"

class Spawnpoint : public WorldObject
{
public:
	Spawnpoint();
	virtual ~Spawnpoint();

	void update(float dt);

private:
	int m_spawnType;
	int m_spawnCount;
	float m_spawnPeriodTime;
	float m_timeTillNextSpawn;
};
