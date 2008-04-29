#pragma once
#include "../utility/Singleton.h"
#include <vector>
#include <string>
class ParserSection;


/**
 * When game loads a list of types is loaded from file,
 * and an integer that represents the type
 */
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

