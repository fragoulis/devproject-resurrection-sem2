#pragma once
#include "Movable.h"
#include "../math/Matrix33.h"



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
	//float variance; // maybe later!
	//float power;
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


private:
	float m_mass;
	Matrix33 m_inertia;
	Matrix33 m_inertiaInverse;
	AirdragData m_airdrag;
	GravityData m_gravity;
	LiftData m_lift;
};
