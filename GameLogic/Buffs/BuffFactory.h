//*****************************************************************************
//**
//**   File:               BuffFactory.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************

#pragma once
#include "../../utility/Singleton.h"
#include <string>
#include <vector>
class Buff;
class ParserSection;


/**
 * Knows how to create a Buff object when given a type
 * Initializes the buff using a prototype
 */
class BuffFactory : public Singleton< BuffFactory >
{
public:

	int getTypeFromName(const std::string& name) const;
	const std::string& getNameFromType(int type) const;

	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();


	Buff* createBuff(int type) const;


private:
	typedef std::vector<std::string> StringVector;
	StringVector m_typeNames;


	typedef std::vector<Buff*> BuffVector;
	BuffVector m_buffPrototypes;


	friend Singleton< BuffFactory >;
	BuffFactory();
	virtual ~BuffFactory();
};
