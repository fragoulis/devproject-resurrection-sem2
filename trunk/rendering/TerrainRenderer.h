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
class LaserRenderer;
class Crater;
class PS_Base;


/**
 * Renders the terrain.
 *
 */

class TerrainRenderer :
	public EventListener< Terrain_Changed >,
	public EventListener< Level_Load >,
	public EventListener< Player_Spawned >,
	public EventListener< Player_Despawned >,
	public EventListener< Player_Destroyed >,
	public EventListener< Player_Respawned >,
	public EventListener< Enemy_Spawned >,
	public EventListener< Enemy_Despawned >,
	public EventListener< Enemy_Destroyed >,
	public EventListener<Key_GoingDown>,
	public EventListener< Ebomb_Spawned >,
	public EventListener< Ebomb_Despawned >,
	public EventListener< Crater_Spawned >,
	public EventListener< Crater_Despawned >,
	public EventListener< Life_Restored >,
	public EventListener< Level_Unload >,
	public EventListener< Level_Complete >

{
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Graphics& g) const;

	void onEvent(Level_Load&);
	void onEvent(Level_Unload&);
	void onEvent(Terrain_Changed&);
	void onEvent(Player_Spawned&);
	void onEvent(Player_Despawned&);
	void onEvent(Player_Destroyed&);
	void onEvent(Player_Respawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Despawned&);
	void onEvent(Enemy_Destroyed&);
	void onEvent(Ebomb_Spawned&);
	void onEvent(Ebomb_Despawned&);
	void onEvent(Crater_Spawned&);
	void onEvent(Crater_Despawned&);
	void onEvent(Life_Restored&);
	void onEvent(Level_Complete&);

	void onEvent(Key_GoingDown&);

	void setCamera(const Camera * cam) {m_cameraRef = cam;}
	void setShipRendererRef( const ShipRenderer * sr) {m_shipRendererRef = sr;}
	void setLaserRendererRef( const LaserRenderer * sr) {m_laserRendererRef = sr;}
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

	struct CraterInfo_t
	{
		const Crater * crater;		// pointer to crater object
		Vector4  color;			// color relative to crater, dep on energy type
		bool	 lifeRestored;
		CraterInfo_t(const Crater * crat,const Vector4& col):lifeRestored(false),crater(crat),
			color(col){}
	};

	// PRIVATE FUNCS

	void _clearResources();
	void _loadResources(const std::string& id,
		                const std::string& gfxlevelfile);

	void _updateTerraform(const float dt); // updates the terraform contribution map
	void _updateTerraformContribution() const;

	void _addShadowCaster(const CoordinateModel& model);
	void _removeShadowCaster(const CoordinateFrame * cf);
	void _renderShadows() const;
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
	Texture * m_lakeNormalTexture;
	Texture * m_lakeTexture;
	Texture * m_heightTexture;
	Texture * m_lakeReflection;			// reflection to applied to the lake
	Vector4	  m_waterColor;

	const ShipRenderer * m_shipRendererRef;		// handle to call when redrawing the ships
	const LaserRenderer * m_laserRendererRef;

	FramebufferObject m_reflectionFBO;
	Renderbuffer m_reflectionDepthBuffer;
	float m_lakeTimer;

	// For the shadows
	std::vector<CoordinateModel> m_shadowCasters;
	PS_Base * m_cloudPS;
	Texture * m_shadowTexture;
	FramebufferObject m_shadowFBO;
	Vector3 m_lightDir;


	// For the craters
	std::vector<CraterInfo_t> m_craterList;
	Texture * m_craterArrowTexture;

};
