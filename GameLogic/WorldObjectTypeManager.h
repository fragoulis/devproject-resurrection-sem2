//*****************************************************************************
//**
//**   File:               WorldObjectTypeManager.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include "../utility/Singleton.h"
#include <vector>
#include <string>
class ParserSection;


class WorldObjectTypeManager : public Singleton< WorldObjectTypeManager >
{
public:
	int getTypeFromName(const std::string& name) const;
	const std::string& getNameFromType(int type) const;

	int getTypeCount() const { return int(m_typeNames.size()); }

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

private:
	typedef std::vector<std::string> StringVector;

	StringVector m_typeNames;



	friend Singleton< WorldObjectTypeManager >;
	WorldObjectTypeManager();
	virtual ~WorldObjectTypeManager();
};

