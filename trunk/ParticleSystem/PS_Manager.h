#pragma once

#include <vector>
#include "../utility/Singleton.h"

class PS_Base;
class VBO;
class ParserSection;
class PS_Manager : public Singleton<PS_Manager>
{
public :
	void clear();
	void init(const ParserSection * parsec);	
	

private : 
	std::vector<PS_Base *>	m_psList;
	VBO					  * m_vbo;

	// singleton stuff
	friend Singleton< PS_Manager >;
	PS_Manager();
	~PS_Manager();
};