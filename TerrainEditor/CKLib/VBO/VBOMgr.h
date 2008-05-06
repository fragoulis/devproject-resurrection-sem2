#pragma once

#include <string>
#include <vector>
#include <assert.h>

#define ATTRIB_NUM 8

namespace CKLib {

/*
	VAttribStatus : 
	Keeps status of attributes. Maximum supported = 8
*/
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

class ParserSection;
class VBO;
class VertexAttribute;
class VBOMgr
{
	typedef struct VBOConf
	{
		std::string name;
		unsigned    indexData,
					vertexData,
					drawUsage;
	};

	std::vector<VBOConf> _configurations;
	std::vector<VBO *>	 _vboList;
	int					 _currentConfig;

	VAttribStatus		 _enabledAttribs;

	static bool _init;
	static VBOMgr * _instance;

	// internal funcs
	void _parseConfiguration(const ParserSection * parsec);	// parses options

	// prohibited funcs
	VBOMgr(const ParserSection * parsec);		// ctor
	~VBOMgr();																// dtor
	VBOMgr(const VBOMgr& mgr){}											// copy ctor
	void operator = (const VBOMgr& mgr){}									// assign op

public :

	void setActiveConfig(const std::string& configname);
	void getActiveConfig(std::string& configname);

	VBO * fetchVBO(std::vector<const VertexAttribute *> attribs,
				   const unsigned maxVertices,
				   const unsigned maxIndices);	// request VBO with specified requirements & current config

	void setCurrentAttribs(const VAttribStatus& attribs) {_enabledAttribs = attribs;};
	const VAttribStatus& getCurrentAttribs() const {return _enabledAttribs;}

	// Singleton Trio Stooges
	static void init(const ParserSection * parsec);	
	static VBOMgr * instance();													
	static void destroy();	

};

#include <VBO/VBOMgr.inl>

}