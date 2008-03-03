#include "Laser.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"


Laser :: Laser() : m_age(0.0f)
{
}

Laser :: ~Laser()
{
}


void Laser :: loadSettings(const ParserSection& ps)
{
}