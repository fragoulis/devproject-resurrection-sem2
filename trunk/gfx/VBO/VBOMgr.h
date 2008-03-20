//*****************************************************************************
//**
//**   File:               VBOMgr.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include <vector>
#include "../../utility/Singleton.h"
#include "../../gfxutils/ConfParser/ParserSection.h"

class VBO;
class VertexAttribute;

/*
	VAttribStatus : 
	Keeps status of attributes. Maximum supported = 8
*/
#define ATTRIB_NUM 8
struct VAttribStatus
{
	union{
		unsigned _raw[2];
		unsigned char attrib[ATTRIB_NUM];
	};
	VAttribStatus(const unsigned data=0x00000000){_raw[0] = _raw[1] = data;}
	VAttribStatus(const unsigned raw1,const unsigned raw2)
	{_raw[0] = raw1;_raw[1] = raw2;}
	VAttribStatus XOR (const VAttribStatus& vs) const
	{return VAttribStatus(_raw[0]^vs._raw[0],_raw[1]^vs._raw[1]);}
	VAttribStatus OR (const VAttribStatus& vs) const
	{return VAttribStatus(_raw[0]|vs._raw[0],_raw[1]|vs._raw[1]);}
	VAttribStatus AND (const VAttribStatus& vs) const
	{return VAttribStatus(_raw[0]&vs._raw[0],_raw[1]&vs._raw[1]);}
	bool operator == (const VAttribStatus& vs) {return (_raw[0]&vs._raw[0]) && (_raw[1]&vs._raw[1]);}
};

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

	VAttribStatus		 m_statusFlags;

public :
	~VBOMgr();

	VBO * fetchVBO(const unsigned usage,
				   const unsigned iSize,
				   const unsigned dsize,
				   const std::vector<const VertexAttribute *> vattrs);

	const VAttribStatus& getCurrentFlags() {return m_statusFlags;}
	void setCurrentFlags(const VAttribStatus& status) {m_statusFlags = status;}

	void init(const ParserSection * parsec);

};