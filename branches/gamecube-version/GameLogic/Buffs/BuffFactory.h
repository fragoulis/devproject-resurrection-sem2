#ifndef _RES_BUFFFACTORY_H_
#define _RES_BUFFFACTORY_H_
#include "../../utility/Singleton.h"
#include <string>
#include <vector>
class Buff;
class ParserSection;


/**
 * Knows how to create a Buff object when given a type
 * Initializes the buff using a prototype
 * Data for each buff is loaded from config/Buffs.txt
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


#endif
