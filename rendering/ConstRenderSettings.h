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
	unsigned m_shadowTextureSize;
	float m_tformTimePerUnit;

	std::string m_laserTexName;
	
	float m_waveChangeRate;
	float m_waveSpeed;
	float m_waveRepeats;
	std::string m_lakeTexture;
	float m_reflectionTextureScreenRatio;

	void _parseMiscEntities(const ParserSection * parsec);

public :
	ConstRenderSettings(){}
	~ConstRenderSettings(){}

	const EntitySettings_t& getEntitySettings(const int type) const;

	const float getSpawnPointInterval() const {return m_spawnpointInterval;}
	const float getSpawnPointSize() const {return m_spawnpointSize;}
	const Texture * getSpawnPointTexture() const {return m_spawnpointTexture;}
	const unsigned getShadowTextureSize() const {return m_shadowTextureSize;}
	const float getTerraformingTimePerUnit() const {return m_tformTimePerUnit;}

	const std::string& getLaserTextureName() const {return m_laserTexName;}

	const float getWaveChangeRate() const{return m_waveChangeRate;}
	const float getWaveSpeed() const{return m_waveSpeed;}
	const std::string& getLakeTexture() const {return m_lakeTexture;}
	const float getWaveRepeats() const {return m_waveRepeats;}
	const float getReflectionTextureScreenRatio() const {return m_reflectionTextureScreenRatio;}

	void init(const ParserSection * parsec);


};