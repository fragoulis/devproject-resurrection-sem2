#pragma once
#include "../WorldObject.h"
#include "../EnergyTypes.h"
class ParserSection;
class Point3;


/**
 * Represents a spawn point for enemies.
 * Enemies spawn in waves (sessions).
 * When a session starts, the portal opens, and spawns a few enemies in quick succession.
 * When the session ends, the portal closes.
 * A minimum and maximum distance to the player can be used to tweak gameplay
 */
class Spawnpoint : public WorldObject
{
public:
	Spawnpoint();
	virtual ~Spawnpoint();

	void update(float dt, const Point3& playerPosition);

	void loadSettings(const ParserSection&);

private:
	int m_spawnType;
	int m_spawnCount;
	EnergyType m_energyType;
	float m_timeBetweenInRangeAndFirstSession;
	float m_timeBetweenSessionStartAndFirstSpawn;
	float m_timeBetweenSpawns;
	float m_timeBetweenLastSpawnAndSessionEnd;
	float m_timeBetweenSessionEndAndSessionStart;
	float m_minimumPlayerDistance;
	float m_maximumPlayerDistance;


	enum State
	{
		WAITING_FOR_PLAYER,
		SESSION_STARTING,
		SPAWNING,
		SESSION_ENDING,
		WAITING_BETWEEN_SESSIONS,
	};

	State m_state;
	float m_timeTillNextEvent;
	int m_enemiesLeftToSpawnThisSession;

	void _spawnEnemy();
};
