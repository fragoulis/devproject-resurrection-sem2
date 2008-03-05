#pragma once
#include "Movable.h"
#include "../math/Matrix33.h"
class ParserSection;


/**
 * Rigidbody extends Movable by adding mass, inertia
 * and physics data for air drag, gravity and lift.
 * This is physics data yes, but it is shared between
 * PhysicsEngine and AIEngine (GameLogic probably never touches it)
 * Data that is shared between engines is stored in game objects.
 * Alternative is to create a bridge between them.
 * That's just too much for this small game (already overdesigning this game).
 *
 * Airdrag, Gravity and Lift data is put into structs for convenience.
 */
struct AirdragData
{
	float factor;
	float coefficient;
};

struct GravityData
{
	float factor;
};

struct LiftData
{
	float factor;
	float variance;
	float power;
};




class Rigidbody : public Movable
{
public:
	Rigidbody();
	virtual ~Rigidbody();

	float getMass() const { return m_mass; }
	const Matrix33& getInertia() const { return m_inertia; }
	const Matrix33& getInertiaInverse() const { return m_inertiaInverse; }
	const AirdragData& getAirdragData() const { return m_airdrag; }
	const GravityData& getGravityData() const { return m_gravity; }
	const LiftData& getLiftData() const { return m_lift; }

	void setMass(float m) { m_mass = m; }
	void setInertia(const Matrix33& i);
	void setAirdragData(const AirdragData& a) { m_airdrag = a; }
	void setGravityData(const GravityData& g) { m_gravity = g; }
	void setLiftData(const LiftData& l) { m_lift = l; }

	void loadSettings(const ParserSection& ps);

private:
	float m_mass;
	Matrix33 m_inertia;
	Matrix33 m_inertiaInverse;
	AirdragData m_airdrag;
	GravityData m_gravity;
	LiftData m_lift;
};
