#pragma once

#include <vector>

struct EntitySettings_t
{
	std::string entityName;
	std::string modelName;
	int type;
	int shaderIndex;
};

class ParserSection;
class ConstRenderSettings
{
	std::vector<EntitySettings_t> m_entities;


public :
	ConstRenderSettings(){}
	~ConstRenderSettings(){}

	const EntitySettings_t& getEntitySettings(const int type) const;

	void init(const ParserSection * parsec);


};