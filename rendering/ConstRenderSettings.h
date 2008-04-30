#pragma once

#include <vector>

/*
	Info struct about entity settings
*/

struct EntitySettings_t
{
	std::string entityName;
	std::string modelName;
	int type;
	int shaderIndex;
};

/*
	Info class that holds data that won't change throughout the app's lifetime
*/

class ParserSection;
class Texture;
class Model;
class ConstRenderSettings
{
	std::vector<EntitySettings_t> m_entities;

	float m_spawnpointInterval;
	float m_spawnpointSize;
	Texture * m_spawnpointTexture;
	Texture * m_crosshairTexture;
	Texture * m_crosshairHelperTexture;
	// other misc stuff
	unsigned m_shadowTextureSize;
	float m_tformTimePerUnit;

	std::string m_laserTexName;
	
	float m_waveChangeRate;
	float m_waveSpeed;
	float m_waveRepeats;
	std::string m_noiseTexture,
				m_noiseTexture1D,
				m_lakeNormalTexture;
	float m_reflectionTextureScreenRatio;

	std::string m_craterArrowTexture;

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
	const std::string& getNoiseTexture() const {return m_noiseTexture;}
	const std::string& getNoiseTexture1D() const {return m_noiseTexture1D;}
	const std::string& getLakeNormalTexture() const {return m_lakeNormalTexture;}
	const std::string& getCraterArrowTexture() const {return m_craterArrowTexture;}
	const float getWaveRepeats(const float mapExtent) const {return m_waveRepeats*mapExtent / 2048.0f;}
	const float getReflectionTextureScreenRatio() const {return m_reflectionTextureScreenRatio;}
	const Texture * getCrosshair() const {return m_crosshairTexture;}
	const Texture * getCrosshairHelper() const {return m_crosshairHelperTexture;}

	void init(const ParserSection * parsec);


};