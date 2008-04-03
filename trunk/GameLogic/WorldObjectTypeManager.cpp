//*****************************************************************************
//**
//**   File:               WorldObjectTypeManager.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#include "WorldObjectTypeManager.h"
#include "../gfxutils/ConfParser/ConfParser.h"
using namespace std;


WorldObjectTypeManager :: WorldObjectTypeManager()
{
}
WorldObjectTypeManager :: ~WorldObjectTypeManager()
{
	onApplicationUnload();
}

int WorldObjectTypeManager :: getTypeFromName(const std::string& name) const
{
	//for (StringVector::const_iterator i = m_typeNames.begin(); i != m_typeNames.end(); ++i)
	for (int i = 0; i != m_typeNames.size(); ++i)
	{
		if (m_typeNames[i] == name) return i;
	}
	assert(0);
	return -1;
}

const string& WorldObjectTypeManager :: getNameFromType(int type) const
{
	return m_typeNames[type];
}


void WorldObjectTypeManager :: onApplicationLoad(const ParserSection& ps)
{
	const ParserSection* psMain = ps.getSection("main");
	m_typeNames = psMain->getValVector("WorldObjectTypes");
}

void WorldObjectTypeManager :: onApplicationUnload()
{
}