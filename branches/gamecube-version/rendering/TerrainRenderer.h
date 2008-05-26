#ifndef _RES_TERRAINRENDERER_H_
#define _RES_TERRAINRENDERER_H_
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
//#include "CoordinateModel.h"
#include "../math/Vector3.h"
#include "../math/Vector4.h"


#include <vector>
#include <string>

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
class TerrainStruct;
class TerrainLoader;
class Texture;
class Lake;


/**
 * Renders the terrain, water and crater arrows
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
	//public EventListener<Key_GoingDown>,
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
	virtual ~TerrainRenderer();

	void render(Graphics& g) const;

	virtual void onEvent(Level_Load&);
	virtual void onEvent(Level_Unload&);
	virtual void onEvent(Terrain_Changed&);
	virtual void onEvent(Player_Spawned&);
	virtual void onEvent(Player_Despawned&);
	virtual void onEvent(Player_Destroyed&);
	virtual void onEvent(Player_Respawned&);
	virtual void onEvent(Enemy_Spawned&);
	virtual void onEvent(Enemy_Despawned&);
	virtual void onEvent(Enemy_Destroyed&);
	virtual void onEvent(Ebomb_Spawned&);
	virtual void onEvent(Ebomb_Despawned&);
	virtual void onEvent(Crater_Spawned&);
	virtual void onEvent(Crater_Despawned&);
	virtual void onEvent(Life_Restored&);
	virtual void onEvent(Level_Complete&);

	//void onEvent(Key_GoingDown&);

	void setCamera(const Camera * cam) {m_cameraRef = cam;}
	//void setShipRendererRef( const ShipRenderer * sr) {m_shipRendererRef = sr;}
	//void setLaserRendererRef( const LaserRenderer * sr) {m_laserRendererRef = sr;}
	void update(const float dt);

	const Vector3& getMapExtents() const {return m_mapExtents;}

	//void updateTerraformContribution() const;
	//void drawLakeReflection(Graphics& g) const;
	//void renderShadows() const;

private:

	// STRUCT DECLARATIONS

	//struct TerraformInfo_t
	//{
	//	Vector3 center;
	//	float radius;
	//	float currentTimeOffset;
	//};

	//struct CraterInfo_t
	//{
	//	const Crater * crater;		// pointer to crater object
	//	Vector4  color;			// color relative to crater, dep on energy type
	//	bool	 lifeRestored;
	//	CraterInfo_t(const Crater * crat,const Vector4& col):lifeRestored(false),crater(crat),
	//		color(col){}
	//};

	// PRIVATE FUNCS

	void _clearResources();
	void _loadResources(const std::string& id,
		                const std::string& gfxlevelfile);

	//void _updateTerraform(const float dt); // updates the terraform contribution map

	//void _addShadowCaster(const CoordinateModel& model);
	//void _removeShadowCaster(const CoordinateFrame * cf);
	//void _initShadows(const Vector4& lightdir);


	// VARIABLES
	const Camera * m_cameraRef;
	Vector4		   m_lightColor;

	Terrain* m_terrain;
	float	* m_heights;
	unsigned m_terrainDimension;
	Vector3  m_mapExtents;

	TerrainStruct * m_terrainStruct;
	Texture		  * m_terrainTextures[2];

	// For terraform contribution computation
	//Texture * m_tformContribTex;
	//std::vector<TerraformInfo_t> m_tformInfo;

	// For the lake
	//Texture * m_lakeNormalTexture;
	//Texture * m_lakeTexture;
	//Texture * m_heightTexture;
	//Texture * m_lakeReflection;			// reflection to applied to the lake
	Vector4	  m_waterColor;

	//const ShipRenderer * m_shipRendererRef;		// handle to call when redrawing the ships
	//const LaserRenderer * m_laserRendererRef;

	//float m_lakeTimer;

	// For the shadows
	//std::vector<CoordinateModel> m_shadowCasters;
	//PS_Base * m_cloudPS;
	//Texture * m_shadowTexture;
	Vector3 m_lightDir;
	Lake  * m_lake;


	// For the craters
	//std::vector<CraterInfo_t> m_craterList;
	//Texture * m_craterArrowTexture;

};


#endif