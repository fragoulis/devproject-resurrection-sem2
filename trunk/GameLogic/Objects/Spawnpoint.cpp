//*****************************************************************************
//**
//**   File:               Spawnpoint.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Spawnpoint.h"
#include "../GameLogic.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../Enemies/EnemyFactory.h"
#include "../../math/Point3.h"
#include "../GameEvents.h"
#include "../../utility/EventManager.h"

Spawnpoint :: Spawnpoint() :
	m_spawnType(-1),
	m_spawnCount(0),
	m_timeBetweenSessionStartAndFirstSpawn(0.0f),
	m_timeBetweenSpawns(0.0f),
	m_timeBetweenLastSpawnAndSessionEnd(0.0f),
	m_timeBetweenSessionEndAndSessionStart(0.0f),
	m_minimumPlayerDistance(0.0f),
	m_maximumPlayerDistance(0.0f),
	m_timeTillNextEvent(0.0f),
	m_enemiesLeftToSpawnThisSession(0),
	m_state(WAITING_FOR_PLAYER)
{
}

Spawnpoint :: ~Spawnpoint()
{
}


void Spawnpoint :: update(float dt, const Point3& playerPosition)
{
	m_timeTillNextEvent -= dt;

	float distance = playerPosition.distance(getPosition());
	bool inRange = distance > m_minimumPlayerDistance && distance < m_maximumPlayerDistance;

	if (!inRange) {
		switch (m_state) {
			case SESSION_STARTING :
			case SPAWNING :
				m_state = SESSION_ENDING;
				m_timeTillNextEvent = m_timeBetweenLastSpawnAndSessionEnd;
				break;
			case SESSION_ENDING :
				if (m_timeTillNextEvent < 0.0f) {
					m_state = WAITING_FOR_PLAYER;
					EventManager::instance().fireEvent(Spawnpoint_SessionEnded(this));
				}
				break;
			case WAITING_BETWEEN_SESSIONS :
				m_state = WAITING_FOR_PLAYER;
				break;
		}
	}
	else
	{
		if (m_state == WAITING_FOR_PLAYER) {
			m_state = WAITING_BETWEEN_SESSIONS;
			m_timeTillNextEvent = m_timeBetweenInRangeAndFirstSession;
		}
		else {
			if (m_timeTillNextEvent < 0.0f)
			{
				switch (m_state) {
					case WAITING_BETWEEN_SESSIONS :
						m_state = SESSION_STARTING;
						m_timeTillNextEvent = m_timeBetweenSessionStartAndFirstSpawn;
						EventManager::instance().fireEvent(Spawnpoint_SessionStarted(this));
						break;
					case SESSION_STARTING :
						m_state = SPAWNING;
						m_enemiesLeftToSpawnThisSession = m_spawnCount;
						_spawnEnemy();
						break;
					case SPAWNING :
						_spawnEnemy();
						break;
					case SESSION_ENDING :
						m_state = WAITING_BETWEEN_SESSIONS;
						m_timeTillNextEvent = m_timeBetweenSessionEndAndSessionStart;
						EventManager::instance().fireEvent(Spawnpoint_SessionEnded(this));
						break;
				}
			}
		}
	}
}

void Spawnpoint :: _spawnEnemy()
{
	GameLogic::instance().spawnEnemies(1, m_spawnType);
	m_enemiesLeftToSpawnThisSession--;
	m_timeTillNextEvent = m_timeBetweenSpawns;
	if (m_enemiesLeftToSpawnThisSession == 0) {
		m_state = SESSION_ENDING;
		m_timeTillNextEvent = m_timeBetweenLastSpawnAndSessionEnd;
	}
}


void Spawnpoint :: loadSettings( const ParserSection& ps )
{
	m_spawnType = EnemyFactory::instance().getTypeFromName(ps.getVal("EnemyType"));
	m_spawnCount = FromString<int>(ps.getVal("EnemyCount"));
	m_timeBetweenInRangeAndFirstSession = FromString<float>(ps.getVal("TimeBetweenInRangeAndFirstSession"));
	m_timeBetweenSessionStartAndFirstSpawn = FromString<float>(ps.getVal("TimeBetweenSessionStartAndFirstSpawn"));
	m_timeBetweenSpawns = FromString<float>(ps.getVal("TimeBetweenSpawns"));
	m_timeBetweenLastSpawnAndSessionEnd = FromString<float>(ps.getVal("TimeBetweenLastSpawnAndSessionEnd"));
	m_timeBetweenSessionEndAndSessionStart = FromString<float>(ps.getVal("TimeBetweenSessionEndAndSessionStart"));
	m_minimumPlayerDistance = FromString<float>(ps.getVal("MinimumPlayerDistance"));
	m_maximumPlayerDistance = FromString<float>(ps.getVal("MaximumPlayerDistance"));

	WorldObject::loadSettings(ps);
}