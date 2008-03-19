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

	std::string m_laserTexName;
	
	float m_waveChangeRate;
	float m_waveSpeed;
	float m_waveRepeats;
	float m_cloudRepeats;
	std::string m_lakeTexture;
	std::string m_cloudTexture;


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

	const std::string& getLaserTextureName() const {return m_laserTexName;}

	const float getWaveChangeRate() const{return m_waveChangeRate;}
	const float getWaveSpeed() const{return m_waveSpeed;}
	const std::string& getLakeTexture() const {return m_lakeTexture;}
	const std::string& getCloudTexture() const {return m_cloudTexture;}
	const float getWaveRepeats() const {return m_waveRepeats;}
	const float getCloudRepeats() const {return m_cloudRepeats;}

	void init(const ParserSection * parsec);


};