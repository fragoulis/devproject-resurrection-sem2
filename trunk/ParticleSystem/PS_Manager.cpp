#include "PS_Manager.h"
#include "PS_Base.h"
#include "../gfx/VBO/VBO.h"

using namespace std;

PS_Manager :: PS_Manager()
:m_vbo(0)
{
}

PS_Manager :: ~PS_Manager()
{
}

void PS_Manager :: clear()
{
	for(vector<PS_Base *>::iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		delete (*it);
		(*it) = 0;
	}
	m_psList.clear();
}