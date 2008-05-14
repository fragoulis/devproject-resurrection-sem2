#pragma once
#include "Buff.h"
class WorldObject;


/**
 * A special buff that slows its target down by a fixed factor.
 * Assumes its target is a spaceship.
 */
class SpeedReductionDebuff : public Buff
{
public:
	SpeedReductionDebuff(int type);
	virtual ~SpeedReductionDebuff();

	virtual void loadSettings(const ParserSection&);
	virtual Buff* clone() const;

private:
	float m_factor;


	virtual void _onActivate(WorldObject* target);
	virtual void _onDeactivate(WorldObject* target);
};
