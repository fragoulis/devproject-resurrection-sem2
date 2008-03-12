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
class Texture;
class Model;
class ConstRenderSettings
{
	std::vector<EntitySettings_t> m_entities;

	float m_spawnpointInterval;
	float m_spawnpointSize;
	Texture * m_spawnpointTexture;
	// other misc stuff
	float m_treeGrowthInterval;
	float m_treeBuriedHeight;


	void _parseMiscEntities(const ParserSection * parsec);

public :
	ConstRenderSettings(){}
	~ConstRenderSettings(){}

	const EntitySettings_t& getEntitySettings(const int type) const;

	const float getSpawnPointInterval() const {return m_spawnpointInterval;}
	const float getSpawnPointSize() const {return m_spawnpointSize;}
	const Texture * getSpawnPointTexture() const {return m_spawnpointTexture;}

	void init(const ParserSection * parsec);


};