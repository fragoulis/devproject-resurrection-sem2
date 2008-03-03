#include "PS_Manager.h"
#include "PS_Base.h"

using namespace std;

PS_Manager :: PS_Manager()
{
}

PS_Manager :: ~PS_Manager()
{
}

void PS_Manager :: clear()
{
	for(vector<PS_Base *>::iterator it = _psList.begin();
		it != _psList.end();
		++it)
	{
		delete (*it);
		(*it) = 0;
	}
	_psList.clear();
}