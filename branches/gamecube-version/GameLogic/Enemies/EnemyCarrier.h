#ifndef _RES_ENEMYCARRIER_H_
#define _RES_ENEMYCARRIER_H_
#include "Enemyship.h"
#include "../EnergyTypes.h"
#include "../../utility/EventManager.h"
#include "../GameEvents.h"



/**
 * Special enemy ship that spawns other enemy ships.
 * Configuration is very similar to spawn points.
 *
 */
class EnemyCarrier :
	public Enemyship,
	public EventListener<Player_Destroyed>,
	public EventListener<Player_Respawned>
{
public:
	EnemyCarrier(int type);
	virtual ~EnemyCarrier();

	void restart();

	virtual void update(float dt);
	virtual Enemyship* clone();

	EnergyType getEnergyTypeSpawns() const { return m_spawnEnergyType; }
	void setEneryTypeSpawns(EnergyType et) { m_spawnEnergyType = et; }

	virtual void loadSettings(const ParserSection&);

	virtual void onEvent(Player_Destroyed&);
	virtual void onEvent(Player_Respawned&);

private:
	int m_spawnType;
	int m_spawnCount;
	EnergyType m_spawnEnergyType;
	float m_timeBetweenInRangeAndFirstSession;
	float m_timeBetweenSessionStartAndFirstSpawn;
	float m_timeBetweenSpawns;
	float m_timeBetweenLastSpawnAndSessionEnd;
	float m_timeBetweenSessionEndAndSessionStart;
	float m_minimumPlayerDistance;
	float m_maximumPlayerDistance;
	bool m_paused;


	enum SpawnState
	{
		WAITING_FOR_PLAYER,
		SESSION_STARTING,
		SPAWNING,
		SESSION_ENDING,
		WAITING_BETWEEN_SESSIONS,
	};

	SpawnState m_spawnState;
	float m_spawnTimeTillNextEvent;
	int m_enemiesLeftToSpawnThisSession;

	void _spawnEnemy();
};


#endif
