#include "Ebomb.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../WorldObjectTypeManager.h"


Ebomb :: Ebomb()
{
	m_state = ALIVE;
	m_radius = 0.0f;
	m_ebombType = EBOMB_TYPE_UNKNOWN;
}

Ebomb :: ~Ebomb()
{
}



void Ebomb :: loadSettings(const ParserSection& ps)
{
	m_radius = FromString<float>(ps.getVal("Radius"));
	Rigidbody::loadSettings(ps);
	setType(WorldObjectTypeManager::instance().getTypeFromName("Ebomb"));
}