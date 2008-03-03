#include "Enemyship.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"

Enemyship :: Enemyship()
{
}

Enemyship :: ~Enemyship()
{
}


void Enemyship :: loadSettings( const ParserSection& ps )
{
	m_energyType = EnergyTypeFromString(ps.getVal("EnergyType"));
	m_collisionPower = FromString<int>(ps.getVal("CollisionPower"));
	Spaceship::loadSettings(ps);
}