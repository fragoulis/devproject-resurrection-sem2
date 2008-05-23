#ifndef _RES_CRATER_H_
#define _RES_CRATER_H_
#include "../WorldObject.h"
#include "../EbombTypes.h"
#include "../EnergyTypes.h"
#include "../GameEvents.h"
#include "../../utility/EventManager.h"
class ParserSection;
class Enemyship;


/**
 * Represents a crater.
 * In physics they are considered circles on the ground.
 * They spawn a protector (an enemy ship).
 * Crater cannot be e-bombed until the protector is destroyed.
 * Crater will respawn its protector after a certain amount of time.
 */
class Crater :
	public WorldObject,
	public EventListener< Enemy_Destroyed >
{
public:

	Crater();
	virtual ~Crater();

	float getRadius() const { return m_radius; }
	EbombType getEbombType() const { return m_ebombType; }
	float getAffectedAreaRadius() const { return m_affectedAreaRadius; }
	bool isProtected() const { return m_protector != 0; }

	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }


	void loadSettings(const ParserSection&);
	void update(float dt);

	virtual void onEvent(Enemy_Destroyed&);

private:
	enum State {
		SPAWNING, // not used yet
		ALIVE,
		DYING, // not used yet
		TO_BE_DELETED,
		UNKNOWN,
	};


	State m_state;
	float m_radius;
	EbombType m_ebombType;
	float m_affectedAreaRadius;
	float m_timeTillNextEvent;

	Enemyship* m_protector;
	float m_protectorRespawnDelay;
	int m_protectorType;
	EnergyType m_protectorEnergyType;
	EnergyType m_protectorCarrierEnergyTypeSpawns;
};


#endif