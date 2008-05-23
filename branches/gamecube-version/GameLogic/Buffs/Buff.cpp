#include "Buff.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"


Buff::Buff(int type) : m_type(type), m_currentStacks(0), m_active(false)
{
}

Buff::~Buff()
{
}

Buff* Buff :: clone() const
{
	return new Buff(*this);
}

void Buff :: update(float dt, WorldObject* target)
{
	m_timeLeft -= dt;
	if (isActive()) {
		_onUpdate(dt, target);
		if (isExpired() || !hasStacks())
		{
			m_active = false;
			m_currentStacks = 0;
			m_timeLeft = 0.0f;
			_onDeactivate(target);
			//cout << "Deactivated buff" << endl;
		}
	}
	else {
		if (!isExpired() && hasStacks())
		{
			m_active = true;
			_onActivate(target);
			//cout << "Activated buff" << endl;
		}
	}
}

void Buff :: loadSettings(const ParserSection& ps)
{
	m_duration = FromString<float>(ps.getVal("Duration"));
	m_maxStacks = FromString<int>(ps.getVal("MaxStacks"));
	m_debuff = FromString<bool>(ps.getVal("Debuff"));

	m_timeLeft = m_duration;
}