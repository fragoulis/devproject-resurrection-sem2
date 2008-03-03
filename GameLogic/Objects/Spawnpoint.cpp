#include "Spawnpoint.h"
#include "../GameLogic.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../Enemies/EnemyFactory.h"

Spawnpoint :: Spawnpoint() :
	m_spawnType(-1),
	m_spawnCount(0),
	m_spawnPeriodTime(0.0f),
	m_timeTillNextSpawn(0.0f)
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
	m_spawnType = EnemyFactory::instance().getTypeFromName(ps.getVal("EnemyType"));
	m_spawnCount = FromString<int>(ps.getVal("EnemyCount"));
	m_spawnPeriodTime = FromString<float>(ps.getVal("TimePeriod"));
	m_timeTillNextSpawn = m_spawnPeriodTime;
	WorldObject::loadSettings(ps);
}