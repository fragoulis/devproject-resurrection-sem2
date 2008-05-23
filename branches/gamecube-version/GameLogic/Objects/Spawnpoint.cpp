#include "Spawnpoint.h"
#include "../GameLogic.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../Enemies/EnemyFactory.h"
#include "../Enemies/Enemyship.h"
#include "../Enemies/EnemyCarrier.h"
#include "../../math/Point3.h"
#include "../GameEvents.h"
#include "../../utility/EventManager.h"
#include "../WorldObjectTypeManager.h"

Spawnpoint :: Spawnpoint() :
	m_spawnType(-1),
	m_spawnCount(0),
	m_timeBetweenInRangeAndFirstSession(0.0f),
	m_timeBetweenSessionStartAndFirstSpawn(0.0f),
	m_timeBetweenSpawns(0.0f),
	m_timeBetweenLastSpawnAndSessionEnd(0.0f),
	m_timeBetweenSessionEndAndSessionStart(0.0f),
	m_minimumPlayerDistance(0.0f),
	m_maximumPlayerDistance(0.0f),
	m_paused(false),
	m_state(WAITING_FOR_PLAYER),
	m_timeTillNextEvent(0.0f),
	m_enemiesLeftToSpawnThisSession(0)
{
	EventManager::instance().registerEventListener<Player_Destroyed>(this);
	EventManager::instance().registerEventListener<Player_Respawned>(this);
}

Spawnpoint :: ~Spawnpoint()
{
	EventManager::instance().unRegisterEventListener<Player_Destroyed>(this);
	EventManager::instance().unRegisterEventListener<Player_Respawned>(this);
}


void Spawnpoint :: restart()
{
	m_paused = false;
	m_state = WAITING_FOR_PLAYER;
}

void Spawnpoint :: update(float dt, const Point3& playerPosition)
{
	if (m_paused) return;

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
					FIRE_EVENT_VAL(Spawnpoint_SessionEnded, this);
				}
				break;
			case WAITING_BETWEEN_SESSIONS :
				m_state = WAITING_FOR_PLAYER;
				break;
			default :
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
						FIRE_EVENT_VAL(Spawnpoint_SessionStarted, this);
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
						FIRE_EVENT_VAL(Spawnpoint_SessionEnded, this);
						break;
					default :
						break;
				}
			} // if (m_timeTillNextEvent < 0.0f)
		} // if (m_state == WAITING_FOR_PLAYER) {
	} // if (!inRange) {
}

void Spawnpoint :: _spawnEnemy()
{
	const Point3& pos = getPosition();
	Enemyship* es = GameLogic::instance().spawnEnemy(m_spawnType, m_energyType, pos.getX(), pos.getZ(), false);

	if (es == 0) return;

	if (m_carrierEnergyTypeSpawns != ENERGY_TYPE_UNKNOWN)
	{
		EnemyCarrier* carrier = static_cast<EnemyCarrier*>(es);
		carrier->setEneryTypeSpawns(m_carrierEnergyTypeSpawns);
	}

	m_enemiesLeftToSpawnThisSession--;
	m_timeTillNextEvent = m_timeBetweenSpawns;
	if (m_enemiesLeftToSpawnThisSession == 0) {
		m_state = SESSION_ENDING;
		m_timeTillNextEvent = m_timeBetweenLastSpawnAndSessionEnd;
	}
}

void Spawnpoint :: onEvent(Player_Destroyed& evt)
{
	m_paused = true;
}

void Spawnpoint :: onEvent(Player_Respawned& evt)
{
	restart();
}



void Spawnpoint :: loadSettings( const ParserSection& ps )
{
	m_spawnType = EnemyFactory::instance().getTypeFromName(ps.getVal("EnemyType"));
	m_spawnCount = FromString<int>(ps.getVal("EnemyCount"));
	m_energyType = EnergyTypeFromString(ps.getVal("EnergyType"));
	m_timeBetweenInRangeAndFirstSession = FromString<float>(ps.getVal("TimeBetweenInRangeAndFirstSession"));
	m_timeBetweenSessionStartAndFirstSpawn = FromString<float>(ps.getVal("TimeBetweenSessionStartAndFirstSpawn"));
	m_timeBetweenSpawns = FromString<float>(ps.getVal("TimeBetweenSpawns"));
	m_timeBetweenLastSpawnAndSessionEnd = FromString<float>(ps.getVal("TimeBetweenLastSpawnAndSessionEnd"));
	m_timeBetweenSessionEndAndSessionStart = FromString<float>(ps.getVal("TimeBetweenSessionEndAndSessionStart"));
	m_minimumPlayerDistance = FromString<float>(ps.getVal("MinimumPlayerDistance"));
	m_maximumPlayerDistance = FromString<float>(ps.getVal("MaximumPlayerDistance"));



	if (EnemyFactory::instance().getEnemyClass(m_spawnType) == "Carrier")
	{
		m_carrierEnergyTypeSpawns = EnergyTypeFromString(ps.getVal("CarrierEnergyTypeSpawns"));
	}
	else
	{
		m_carrierEnergyTypeSpawns = ENERGY_TYPE_UNKNOWN;
	}


	WorldObject::loadSettings(ps);

	setType(WorldObjectTypeManager::instance().getTypeFromName("Spawnpoint"));
}