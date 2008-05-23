#include "EnemyCarrier.h"
#include "../GameLogic.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../Enemies/EnemyFactory.h"
#include "../Enemies/Enemyship.h"
#include "../../math/Point3.h"
#include "../GameEvents.h"
#include "../../utility/EventManager.h"
#include "../WorldObjectTypeManager.h"
#include "../Objects/Playership.h"
#include <iostream>
using namespace std;

EnemyCarrier :: EnemyCarrier(int type) :
	Enemyship(type),
	m_spawnType(-1),
	m_spawnCount(0),
	m_timeBetweenSessionStartAndFirstSpawn(0.0f),
	m_timeBetweenSpawns(0.0f),
	m_timeBetweenLastSpawnAndSessionEnd(0.0f),
	m_timeBetweenSessionEndAndSessionStart(0.0f),
	m_minimumPlayerDistance(0.0f),
	m_maximumPlayerDistance(0.0f),
	m_spawnState(WAITING_FOR_PLAYER),
	m_spawnTimeTillNextEvent(0.0f),
	m_enemiesLeftToSpawnThisSession(0)
{
	EventManager::instance().registerEventListener<Player_Destroyed>(this);
	EventManager::instance().registerEventListener<Player_Respawned>(this);
}

EnemyCarrier :: ~EnemyCarrier()
{
	EventManager::instance().unRegisterEventListener<Player_Destroyed>(this);
	EventManager::instance().unRegisterEventListener<Player_Respawned>(this);
}

Enemyship* EnemyCarrier :: clone()
{
	return new EnemyCarrier(*this);
}

void EnemyCarrier :: restart( )
{
	m_spawnState = WAITING_FOR_PLAYER;
	m_paused = false;
}


void EnemyCarrier :: update(float dt)
{
	Enemyship::update(dt);

	const Playership* player = GameLogic::instance().getPlayership();
	const Point3& playerPosition = player->getPosition();
	m_spawnTimeTillNextEvent -= dt;

	float distance = playerPosition.distance(getPosition());
	bool inRange = distance > m_minimumPlayerDistance && distance < m_maximumPlayerDistance;

	if (!inRange) {
		switch (m_spawnState) {
			case SESSION_STARTING :
			case SPAWNING :
				m_spawnState = SESSION_ENDING;
				m_spawnTimeTillNextEvent = m_timeBetweenLastSpawnAndSessionEnd;
				break;
			case SESSION_ENDING :
				if (m_spawnTimeTillNextEvent < 0.0f) {
					m_spawnState = WAITING_FOR_PLAYER;
					FIRE_EVENT_VAL(Carrier_SessionEnded, this);
				}
				break;
			case WAITING_BETWEEN_SESSIONS :
				m_spawnState = WAITING_FOR_PLAYER;
				break;
			default :
				break;
		}
	}
	else
	{
		if (m_spawnState == WAITING_FOR_PLAYER) {
			m_spawnState = WAITING_BETWEEN_SESSIONS;
			m_spawnTimeTillNextEvent = m_timeBetweenInRangeAndFirstSession;
		}
		else {
			if (m_spawnTimeTillNextEvent < 0.0f)
			{
				switch (m_spawnState) {
					case WAITING_BETWEEN_SESSIONS :
						m_spawnState = SESSION_STARTING;
						m_spawnTimeTillNextEvent = m_timeBetweenSessionStartAndFirstSpawn;
						FIRE_EVENT_VAL(Carrier_SessionStarted, this);
						break;
					case SESSION_STARTING :
						m_spawnState = SPAWNING;
						m_enemiesLeftToSpawnThisSession = m_spawnCount;
						_spawnEnemy();
						break;
					case SPAWNING :
						_spawnEnemy();
						break;
					case SESSION_ENDING :
						m_spawnState = WAITING_BETWEEN_SESSIONS;
						m_spawnTimeTillNextEvent = m_timeBetweenSessionEndAndSessionStart;
						FIRE_EVENT_VAL(Carrier_SessionEnded, this);
						break;
					default :
						break;
				}
			}
		}
	}
}

void EnemyCarrier :: _spawnEnemy()
{
	const Point3& pos = getPosition();
	Enemyship* es = GameLogic::instance().spawnEnemy(m_spawnType, m_spawnEnergyType, pos.getX(), pos.getZ() + getRadius(), false);
	if (es == 0) return;

	m_enemiesLeftToSpawnThisSession--;
	m_spawnTimeTillNextEvent = m_timeBetweenSpawns;
	if (m_enemiesLeftToSpawnThisSession == 0) {
		m_spawnState = SESSION_ENDING;
		m_spawnTimeTillNextEvent = m_timeBetweenLastSpawnAndSessionEnd;
	}
}


void EnemyCarrier :: onEvent(Player_Destroyed& evt)
{
	m_paused = true;
}

void EnemyCarrier :: onEvent(Player_Respawned& evt)
{
	restart();
}


void EnemyCarrier :: loadSettings( const ParserSection& ps )
{
	m_spawnType = EnemyFactory::instance().getTypeFromName(ps.getVal("EnemyType"));
	m_spawnCount = FromString<int>(ps.getVal("EnemyCount"));
	m_spawnEnergyType = EnergyTypeFromString(ps.getVal("SpawnedEnergyType"));
	m_timeBetweenInRangeAndFirstSession = FromString<float>(ps.getVal("TimeBetweenInRangeAndFirstSession"));
	m_timeBetweenSessionStartAndFirstSpawn = FromString<float>(ps.getVal("TimeBetweenSessionStartAndFirstSpawn"));
	m_timeBetweenSpawns = FromString<float>(ps.getVal("TimeBetweenSpawns"));
	m_timeBetweenLastSpawnAndSessionEnd = FromString<float>(ps.getVal("TimeBetweenLastSpawnAndSessionEnd"));
	m_timeBetweenSessionEndAndSessionStart = FromString<float>(ps.getVal("TimeBetweenSessionEndAndSessionStart"));
	m_minimumPlayerDistance = FromString<float>(ps.getVal("MinimumPlayerDistance"));
	m_maximumPlayerDistance = FromString<float>(ps.getVal("MaximumPlayerDistance"));

	Enemyship::loadSettings(ps);
}