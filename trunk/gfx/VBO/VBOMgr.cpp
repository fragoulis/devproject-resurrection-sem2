//*****************************************************************************
//**
//**   File:               VBOMgr.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "VBOMgr.h"
#include "VBO.h"
#include "../../gfxutils/Misc/utils.h"


VBOMgr :: ~VBOMgr()
{
	for(vbovec::iterator it = m_list.begin(); it != m_list.end(); ++it)
		delete *it;
	m_list.clear();
}

void VBOMgr :: init(const ParserSection * parsec)
{
	m_statusFlags = 0xFFFFFFFF;
	m_settings[U_STATIC].dSize = FromString<unsigned>(parsec->getVal("StaticVData"));
	m_settings[U_STATIC].iSize = FromString<unsigned>(parsec->getVal("StaticIndices"));
	m_settings[U_DYNAMIC].dSize = FromString<unsigned>(parsec->getVal("DynamicVData"));
	m_settings[U_DYNAMIC].iSize = FromString<unsigned>(parsec->getVal("DynamicIndices"));
	m_settings[U_STREAM].dSize = FromString<unsigned>(parsec->getVal("StreamVData"));
	m_settings[U_STREAM].iSize = FromString<unsigned>(parsec->getVal("StreamIndices"));
}

VBO * VBOMgr :: fetchVBO(const unsigned usage,
				   const unsigned iSize,
				   const unsigned dsize,
				   const std::vector<const VertexAttribute *> vattrs)
{
	// Create a VBO ptr & vert & ind offsets
	unsigned voffset(0),
			 ioffset(0);
	VBO * vbo = NULL;
	bool ok(false);

	const int usageIndex = (GL_STATIC_DRAW) ? 0 : (GL_DYNAMIC_DRAW) ? 1 : (GL_STREAM_DRAW) ? 2 : -1;
	assert(usageIndex != -1);

	for(size_t i=0;i< m_list.size();++i)
	{
		vbo = m_list[i];
		// Check usage
		if(vbo->drawUsage() == usage)
		{
			unsigned found(0);
			for(unsigned j=0;j<vattrs.size();++j)
				for(unsigned k=0;k<vbo->attrib_num();++k)
					if(vbo->attrib(k) == vattrs[j])
						++found;
			if((found == vattrs.size()) && 
			   (found == vbo->attrib_num()) && 
				vbo->querySize(dsize,iSize))
			{
				ok = true;
				i = m_list.size();
				break;
			}
		}
	}
	// If no matching VBO found, create a new one
	if(!ok)
	{
		//create based on characteristics
		vbo = new VBO(vattrs,
					  m_settings[usageIndex].dSize,
					  m_settings[usageIndex].iSize,
					  usage,
					  "Whatever");
		m_list.push_back(vbo);
	}
	return vbo;
}