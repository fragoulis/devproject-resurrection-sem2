#ifndef _RES_PS_MANAGER_H_
#define _RES_PS_MANAGER_H_

#include <string>
#include <vector>
#include "../utility/Singleton.h"

class PS_Base;
//class VBO;
class ParserSection;

/*
	Particle system manager : 
	Creates & stores initially the particle systems, & afterwards it fetches tham when queried to do so.
*/

class PS_Manager : public Singleton<PS_Manager>
{
public :
	void clear();
	void init(const ParserSection * parsec);	

	// fetchNewPS : gets an dynamically allocated PS based on a random template, fast - no worries
	// if needed , the mem allocator could be changed to the faster MemMgrAny<>, but not really needed.
	PS_Base * fetchNewPS(const std::string& name); 
	

private : 
	struct PSNameVariations
	{
		std::string name;				// name of ps
		unsigned	variations;			// number of variations stored
		unsigned	posInList;			// first index in list
		PSNameVariations(const std::string& n, const unsigned var,const unsigned pos)
			:name(n),variations(var),posInList(pos){}
	};

	std::vector<PS_Base *>	m_psList;
	std::vector<PSNameVariations> m_psDescList;
	//VBO					  * m_vbo;

	// singleton stuff
	friend Singleton< PS_Manager >;
	PS_Manager();
	~PS_Manager();
};

#endif