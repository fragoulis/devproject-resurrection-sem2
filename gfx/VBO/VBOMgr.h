#pragma once

#include <vector>
#include "../../utility/Singleton.h"
#include "../../gfxutils/ConfParser/ParserSection.h"

class VBO;
class VertexAttribute;
class VBOMgr : public Singleton<VBOMgr>
{
	// Internal Declarations
	typedef std::vector<VBO *> vbovec;
	struct UsageMaxData_t
	{
		unsigned iSize,
				 dSize;
	};
	enum UsageMode_t
	{
		U_STATIC =0,
		U_DYNAMIC,
		U_STREAM,
	};

	vbovec				 m_list;
	UsageMaxData_t		 m_settings[3];

public :
	~VBOMgr();

	VBO * fetchVBO(const unsigned usage,
				   const unsigned iSize,
				   const unsigned dsize,
				   const std::vector<const VertexAttribute *> vattrs);

	void init(const ParserSection * parsec);

};