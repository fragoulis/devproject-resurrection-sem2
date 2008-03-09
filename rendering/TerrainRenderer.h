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
#include "CoordinateModel.h"

#include <vector>

class Terrain;
class Graphics;
class Model;
class VBO;

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
		                const std::string& dataFileName,
						const std::string& textureFileName);

	Terrain* m_terrain;
	Model     * m_terrainModel;			// The terrain 'model'
	VBO       * m_vbo;					// The terrain will always have it's own VBO

	std::vector<CoordinateModel> m_shadowCasters;

	// Put any extra data that is needed goes here
};