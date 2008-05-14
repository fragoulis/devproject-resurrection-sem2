#pragma once
#include "../Spaceship.h"
#include "../EnergyTypes.h"
#include "../Buffs/BuffContainer.h"
class Point3;
class Point2;



/**
 * Represents the player's spaceship.
 * Has buffs and stores energy.
 *
 */
class Playership : public Spaceship
{
public:
	Playership();
	virtual ~Playership();

	void update(float dt);
	void confine(const Point2& minPoint, const Point2& maxPoint);

	int getEnergy(EnergyType type) const { return m_energy[type]; }
	void setEnergy(EnergyType type, int amount) { m_energy[type] = amount; }

	int getEnergyCapacity() const { return m_energyCapacity; }
	int getTotalEnergy();

	void resetAllEnergy();
	void respawn();
	void setDying() { m_state = DYING; }
	void setAlive() { m_state = ALIVE; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }
	bool isDying() const { return m_state == DYING; }
	bool isAlive() const { return m_state == ALIVE; }
	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }

	bool isInvulnerable() const { return hasBuff(m_bufftypeInvulnerability); }
	float getInvulnerableTimeLeft() const { return getBuffTimeLeft(m_bufftypeInvulnerability); }
	void makeInvulnerable() { addBuff(m_bufftypeInvulnerability); }
	float getInvulnerableMaxTime() const { return getBuffMaxTime(m_bufftypeInvulnerability); }

	void addBuff(int type) { m_buffContainer.addBuff(type); }
	bool hasBuff(int type) const { return m_buffContainer.hasBuff(type); }
	int getBuffStacks(int type) const { return m_buffContainer.getBuffStacks(type); }
	float getBuffTimeLeft(int type) const { return m_buffContainer.getBuffTimeLeft(type); }
	float getBuffMaxTime(int type) const { return m_buffContainer.getBuffDuration(type); }
    const Buff *getBuff(int type) const { return m_buffContainer.getBuff(type); }

	void loadSettings(const ParserSection&);

private:
	enum State {
		ALIVE,
		DYING,
		TO_BE_DELETED,
		UNKNOWN,
	};

	State m_state;
	float m_timeTillNextEvent;
	float m_respawnInvulnerableTime;
	int m_energy[ENERGY_TYPE_COUNT];
	int m_energyCapacity;
	BuffContainer m_buffContainer;
	int m_bufftypeInvulnerability;
};
