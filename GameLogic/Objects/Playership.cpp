#include "Playership.h"

Playership :: Playership()
{
	for (int i = 0; i < ENERGY_TYPE_COUNT; i++)
		m_energy[i] = 0;
}

Playership :: ~Playership()
{
}
