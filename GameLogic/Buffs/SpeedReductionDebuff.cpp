#include "SpeedReductionDebuff.h"
#include "../Spaceship.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"

SpeedReductionDebuff :: SpeedReductionDebuff(int type) : Buff(type)
{
}

SpeedReductionDebuff :: ~SpeedReductionDebuff()
{
}

Buff* SpeedReductionDebuff :: clone() const
{
	return new SpeedReductionDebuff(*this);
}

void SpeedReductionDebuff :: _onActivate(WorldObject* target)
{
	Spaceship* ship = static_cast<Spaceship*>(target);
	ship->multiplyThrusterFactor(1.0f / m_factor);
}

void SpeedReductionDebuff :: _onDeactivate(WorldObject* target)
{
	Spaceship* ship = static_cast<Spaceship*>(target);
	ship->multiplyThrusterFactor(m_factor);
}


void SpeedReductionDebuff :: loadSettings(const ParserSection& ps)
{
	m_factor = FromString<float>(ps.getVal("Factor"));

	Buff::loadSettings(ps);
}