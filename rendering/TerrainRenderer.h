//*****************************************************************************
//**
//**   File:               TerrainRenderer.h
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-19 20:39:19 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 200 $
//**
//*****************************************************************************

#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/FBO/Renderbuffer.h"
#include "../gfx/Material.h"
#include "CoordinateModel.h"
#include "../math/Vector3.h"
#include "../math/Vector4.h"

#include "../control/OSinterface/Input.h"

#include <vector>

class Terrain;
class Graphics;
class Model;
class VBO;
class Texture;
class Camera;
class ShipRenderer;

class TerrainRenderer :
	public EventListener< Terrain_Changed >,
	public EventListener< Level_Load >,
	public EventListener< Player_Spawned >,
	public EventListener< Enemy_Spawned >,
	public EventListener< Enemy_Despawned >,
	public EventListener< Player_Despawned >,
	public EventListener<Key_GoingDown>

{
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Graphics& g) const;

	void onEvent(Level_Load&);
	void onEvent(Terrain_Changed&);
	void onEvent(Player_Spawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Despawned&);
	void onEvent(Player_Despawned&);

	void onEvent(Key_GoingDown&);

	void setCamera(const Camera * cam) {m_cameraRef = cam;}
	void setTransparentReflection(Texture * tex) {m_transpReflection = tex;}
	void setShipRendererRef( const ShipRenderer * sr) {m_shipRendererRef = sr;}
	void update(const float dt);

	const Vector3& getMapExtents() const {return m_mapExtents;}

private:

	// STRUCT DECLARATIONS

	struct TerraformInfo_t
	{
		Vector3 center;
		float radius;
		float currentTimeOffset;
	};

	// PRIVATE FUNCS

	void _clearResources();
	void _loadResources(const std::string& id,
		                const std::string& gfxlevelfile);

	void _updateTerraform(const float dt); // updates the terraform contribution map
	void _updateTerraformContribution() const;

	void _addShadowCaster(const CoordinateModel& model);
	void _removeShadowCaster(const CoordinateFrame * cf);
	void _renderShadows2() const;
	void _initShadows(const Vector4& lightdir);
	void _drawLakeReflection(Graphics& g) const;


	// VARIABLES
	const Camera * m_cameraRef;
	Vector4		   m_lightColor;

	Terrain* m_terrain;
	float	* m_heights;
	unsigned m_terrainDimension;
	Vector3  m_mapExtents;
	Model     * m_terrainModel;			// The terrain 'model'
	VBO       * m_vbo;					// The terrain will always have it's own VBO

	// For terraform contribution computation
	FramebufferObject m_tformFBO;
	Texture * m_tformContribTex;
	std::vector<TerraformInfo_t> m_tformInfo;

	// For the lake
	Texture * m_lakeTexture;
	Texture * m_heightTexture;
	Texture * m_transpReflection;		// gotten from WorldRenderer
	Texture * m_lakeReflection;			// reflection to applied to the lake
	const ShipRenderer * m_shipRendererRef;		// handle to call when redrawing the ships
	FramebufferObject m_reflectionFBO;
	Renderbuffer m_reflectionDepthBuffer;
	float m_lakeTimer;

	// For the shadows
	std::vector<CoordinateModel> m_shadowCasters;
	Texture * m_shadowTexture;
	FramebufferObject m_shadowFBO;
	Vector3 m_lightDir;

};
