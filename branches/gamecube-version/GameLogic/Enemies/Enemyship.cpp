
#include "Enemyship.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../Objects/Playership.h"
#include "../../utility/EventManager.h"
#include "../GameEvents.h"

Enemyship :: Enemyship(int type)
{
	m_state = ALIVE;
	setType(type);
}

Enemyship :: ~Enemyship()
{
}

void Enemyship :: update(float dt)
{
}

Enemyship* Enemyship :: clone()
{
	return new Enemyship(*this);
}


void Enemyship :: loadSettings( const ParserSection& ps )
{
	m_energyType = EnergyTypeFromString(ps.getVal("EnergyType"));
	m_collisionPower = FromString<int>(ps.getVal("CollisionPower"));
	m_hitPoints = float(FromString<int>(ps.getVal("HitPoints")));
	m_energyPoints = FromString<int>(ps.getVal("EnergyPoints"));
	Spaceship::loadSettings(ps);
}

void Enemyship :: collideWithPlayer(Playership* player)
{
	// remove enemy from the game
	setToBeDeleted();

	if (player->isDying()) return;

	// Fire destroyed event
	FIRE_EVENT_VAL(Enemy_Destroyed, this);

	// check if player is invulnerable
	if (player->isInvulnerable()) return;

	// get player energy, reduce it by enemy's collision power
	EnergyType type = getEnergyType();
	int enemyPower = getCollisionPower();
	int playerEnergy = player->getEnergy(type) - enemyPower;

	// check if player can sustain the damage taken
	if (playerEnergy < 0) {
		FIRE_EVENT_2VALS(Player_Destroyed, player, type);
	}
	else {
		player->setEnergy(type, playerEnergy);
		FIRE_EVENT_3VALS(Player_EnergyDrained, player, type, enemyPower);
	}
}