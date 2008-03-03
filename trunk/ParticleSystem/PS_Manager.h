#pragma once

#include <vector>
#include "../utility/Singleton.h"

class PS_Base;
class PS_Manager : public Singleton<PS_Manager>
{
public :
	void clear();
	

private : 
	std::vector<PS_Base *>	_psList;

	// singleton stuff
	friend Singleton< PS_Manager >;
	PS_Manager();
	~PS_Manager();
};