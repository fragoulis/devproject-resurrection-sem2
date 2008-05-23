#include "Spaceship.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../gfxutils/Misc/utils.h"

Spaceship :: Spaceship()
{
	m_radius = 0.0f;
	m_thrusterData.power = 0.0f;
	m_thrusterData.maxPower = 0.0f;
	m_thrusterData.factor = 1.0f;
    m_thrusterData.direction.set( 0.0f, 0.0f, -1.0f );
}

Spaceship :: ~Spaceship()
{
}


void Spaceship :: loadSettings(const ParserSection& ps)
{
	m_thrusterData.maxPower = FromString<float>(ps.getVal("ThrusterMaxPower"));
	m_radius = FromString<float>(ps.getVal("Radius"));
	Rigidbody::loadSettings(ps);

	//float I = 2.0f / 5.0f * getMass() * m_radius * m_radius;
	//setInertia(Matrix33(I));
}