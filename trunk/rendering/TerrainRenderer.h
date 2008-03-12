//*****************************************************************************
//**
//**   File:               TerrainRenderer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/Material.h"
#include "CoordinateModel.h"
#include "../math/Vector3.h"

#include <vector>

class Terrain;
class Graphics;
class Model;
class VBO;
class Texture;

class TerrainRenderer :
	public EventListener< Terrain_Changed >,
	public EventListener< Level_Load >
{
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Graphics& g) const;

	void onEvent(Level_Load&);
	void onEvent(Terrain_Changed&);

	void addShadowCaster(const CoordinateModel& model);
	void removeShadowCaster(const CoordinateModel& model);

private:
	void _clearResources();
	void _loadResources(const std::string& id,
		                const std::string& gfxlevelfile);

	Terrain* m_terrain;
	float	* m_heights;
	unsigned m_terrainDimension;
	Vector3  m_mapExtents;
	Model     * m_terrainModel;			// The terrain 'model'
	VBO       * m_vbo;					// The terrain will always have it's own VBO

	// For terraform contribution computation
	FramebufferObject m_tformFBO;
	Texture * m_tformContribTex;

	// For the lake
	Texture * m_lakeTexture;

	// For the shadow casters
	std::vector<CoordinateModel> m_shadowCasters;

	// For the trees
	/*
		while barren activated = false
		while terraforming, activated = true, the positions will adjust a bit
		after terraforming, just draw
	*/
	struct TreeInfo_t
	{
		Vector3 position;
		bool	isActivated;
		bool    isTerraforming;
		float   terraformElapsed;
		
		TreeInfo_t(const Vector3& pos)
			:position(pos),isActivated(false),isTerraforming(false),terraformElapsed(0.0f){}
	};

	struct ForestInfo_t
	{
		std::vector<TreeInfo_t> trees;
		const Model * modelGeom;
		const Model * modelTex;
		ForestInfo_t(const Model * mg, const Model * mt)
			:modelGeom(mg),modelTex(mt){}
	};

	Material m_treeMaterial;		// 0.0 - 1.0 ambient, 1.0 diffuse, 0 specular?
	std::vector<ForestInfo_t>	m_trees;
};
