#include "Spawnpoint.h"
#include "GameLogic.h"

Spawnpoint :: Spawnpoint()
{
}

Spawnpoint :: ~Spawnpoint()
{
}


void Spawnpoint :: update(float dt)
{
	m_timeTillNextSpawn -= dt;

	if (m_timeTillNextSpawn < 0.0f) {
		GameLogic::instance().spawnEnemies(m_spawnCount, m_spawnType);
		m_timeTillNextSpawn = m_spawnPeriodTime;
	}
}