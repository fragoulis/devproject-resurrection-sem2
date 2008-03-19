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
	unsigned m_shadowTextureSize;
	float m_tformTimePerUnit;


	void _parseMiscEntities(const ParserSection * parsec);

public :
	ConstRenderSettings(){}
	~ConstRenderSettings(){}

	const EntitySettings_t& getEntitySettings(const int type) const;

	const float getSpawnPointInterval() const {return m_spawnpointInterval;}
	const float getSpawnPointSize() const {return m_spawnpointSize;}
	const Texture * getSpawnPointTexture() const {return m_spawnpointTexture;}
	const float getTreeBuriedHeight() const {return m_treeBuriedHeight;}
	const unsigned getShadowTextureSize() const {return m_shadowTextureSize;}
	const float getTerraformingTimePerUnit() const {return m_tformTimePerUnit;}

	void init(const ParserSection * parsec);


};