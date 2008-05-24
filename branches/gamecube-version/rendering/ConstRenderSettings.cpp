#include "ConstRenderSettings.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../gfxutils/Model/ModelMgr.h"
//#include "../gfx/Model/Model.h"
//#include "../gfx/Shaders/ShaderManager.h"
//#include "../gfx/Model/ModelMgr.h"
//#include "../gfx/Texture/TextureIO.h"
#include "../GameLogic/WorldObjectTypeManager.h"
#include "../gfxutils/Misc/utils.h"
#include "../utility/assert.h"

void ConstRenderSettings :: init(const ParserSection * parsec)
{
	std::vector<const ParserSection *> entities = parsec->getChildren();
	for(size_t i=0;i<entities.size();++i)
	{
		if(entities[i]->getName() == "Misc")
		{
			_parseMiscEntities(entities[i]);
			continue;
		}
		EntitySettings_t es;
		es.entityName = entities[i]->getName();
		es.modelName = entities[i]->getVal("ModelHook");
		es.type = WorldObjectTypeManager::instance().getTypeFromName(es.entityName);
		OSReport("DBG val : %d\n",es.type);
		
		ModelMgr::instance().getModel(es.modelName);

		// push back the settings
		m_entities.push_back(es);
	}
}

const EntitySettings_t& ConstRenderSettings :: getEntitySettings(const int type) const
{
	for(std::vector<EntitySettings_t>::const_iterator it = m_entities.begin();
		it != m_entities.end();
		++it)
	{
		if(it->type == type)
			return (*it);
	}
	assert(0);
	return (*m_entities.begin());	// shut up compiler!
}

void ConstRenderSettings :: _parseMiscEntities(const ParserSection * parsec)
{
	// read spawn point info
	m_spawnpointInterval = FromString<float>(parsec->getVal("SpawnPointInterval"));
	m_spawnpointSize = FromString<float>(parsec->getVal("SpawnPointSize"));
	//m_spawnpointTexture = TextureIO::instance()->getTexture(parsec->getVal("SpawnPointTexture"));

	// shadow info
	m_shadowTextureSize = FromString<unsigned>(parsec->getVal("ShadowTextureSize"));

	// terraform info
	m_tformTimePerUnit = FromString<float>(parsec->getVal("TerraformingTimePerUnit"));

	// laser info
	m_laserTexName = parsec->getVal("LaserTexture");

	// lake info
	m_waveChangeRate = FromString<float>(parsec->getVal("WaveChangeRate"));
	m_waveSpeed = FromString<float>(parsec->getVal("WaveSpeed"));
	m_waveRepeats = FromString<float>(parsec->getVal("WaveRepeats"));
	m_noiseTexture = parsec->getVal("NoiseTexture");
	m_noiseTexture1D = parsec->getVal("NoiseTexture1D");
	m_lakeNormalTexture = parsec->getVal("LakeNormalTexture");
	m_reflectionTextureScreenRatio = FromString<float>(parsec->getVal("ReflectionTextureSizeRatio"));

	m_craterArrowTexture = parsec->getVal("CraterArrowTexture");
	//m_crosshairTexture = TextureIO::instance()->getTexture(parsec->getVal("CrosshairTexture"));
	//m_crosshairHelperTexture = TextureIO::instance()->getTexture(parsec->getVal("CrosshairHelperTexture"));
	
}