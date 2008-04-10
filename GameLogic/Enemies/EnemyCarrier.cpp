//*****************************************************************************
//**
//**   File:               EnemyCarrier.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 15:18:08 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 212 $
//**
//*****************************************************************************


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
	m_spawnTimeTillNextEvent(0.0f),
	m_enemiesLeftToSpawnThisSession(0),
	m_spawnState(WAITING_FOR_PLAYER)
{
}

EnemyCarrier :: ~EnemyCarrier()
{
}

Enemyship* EnemyCarrier :: clone()
{
	return new EnemyCarrier(*this);
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
					EventManager::instance().fireEvent(Carrier_SessionEnded(this));
				}
				break;
			case WAITING_BETWEEN_SESSIONS :
				m_spawnState = WAITING_FOR_PLAYER;
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
						EventManager::instance().fireEvent(Carrier_SessionStarted(this));
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
						EventManager::instance().fireEvent(Carrier_SessionEnded(this));
						break;
				}
			}
		}
	}
}

void EnemyCarrier :: _spawnEnemy()
{
	Enemyship* es = GameLogic::instance().spawnEnemy(m_spawnType);
	Point3 pos = getPosition();
	pos.add(Vector3(0.0f, 0.0f, getRadius()));
	es->setPosition(pos);
	es->setEnergyType(m_spawnEnergyType);
	EventManager::instance().fireEvent(Enemy_Spawned(es));

	m_enemiesLeftToSpawnThisSession--;
	m_spawnTimeTillNextEvent = m_timeBetweenSpawns;
	if (m_enemiesLeftToSpawnThisSession == 0) {
		m_spawnState = SESSION_ENDING;
		m_spawnTimeTillNextEvent = m_timeBetweenLastSpawnAndSessionEnd;
	}
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