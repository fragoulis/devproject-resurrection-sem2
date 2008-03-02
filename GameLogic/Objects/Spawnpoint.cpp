#include "Spawnpoint.h"
#include "../GameLogic.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"

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


void Spawnpoint :: loadSettings( const ParserSection& ps )
{
	m_spawnType = FromString<int>(ps.getVal("EnemyType"));
	m_spawnCount = FromString<int>(ps.getVal("EnemyCount"));
	m_spawnPeriodTime = FromString<float>(ps.getVal("TimePeriod"));
	WorldObject::loadSettings(ps);
}