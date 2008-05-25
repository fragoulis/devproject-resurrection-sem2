#ifndef _RES_WORLDRENDERER_H_
#define _RES_WORLDRENDERER_H_
#include "IRenderer.h"
#include "TerrainRenderer.h"
#include "SpawnPointRenderer.h"
#include "ParticleSystemsRenderer.h"
#include "LaserRenderer.h"
#include "ShipRenderer.h"
//#include "ClampRenderer.h"
#include "../Math/Vector3.h"
#include "../GameLogic/Objects/Playership.h"



class Camera;
class CoordinateFrame;
class Texture;
class IPostProcessFX;

/**
 * Renders the world!
	
	world = superset of : particle systems
						  misc FX
						  clamping effect
						  ships
						  terrain / water / crater arrows
						  lasers
						  spawn points

    uses 2 cameras (1 for rendering)
 *
 */


class WorldRenderer : public IRenderer,  
					  public EventListener< Player_Spawned >,
					  public EventListener< Player_Despawned >,
					  public EventListener< Level_Unload >,
					  public EventListener< Level_Complete >
{
public:
	WorldRenderer();
	virtual ~WorldRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	const Vector3& getLightDir() const {return m_lightDir;}
	void setLightDir(const Vector3& ld) {m_lightDir.set(ld.getX(),ld.getY(),ld.getZ());}
	Camera * getCamera() {return m_realCam;}
	void newCamera(Camera * cam);		// the argument must be new-d before passing


	// stored Matrices & stuff
	const double * getModelViewMatrixd() const {return m_mviewd;}
	const double * getProjectionMatrixd() const {return m_projd;}
	const float * getModelViewMatrixf() const {return m_mviewf;}
	const float * getProjectionMatrixf() const {return m_projf;}

	void computeBounds();	// compute player & camera bounds for the level



	// Events
	void onEvent(Player_Spawned&);
	void onEvent(Player_Despawned&);
	void onEvent(Level_Unload&);
	void onEvent(Level_Complete&);

	void togglePostProc() {m_postProcOn = !m_postProcOn;}
	void toggleWireframe() {m_wireframeOn = !m_wireframeOn;}

	void adjustCameras();

private:

	void _updateMatrices();					// updates modelview & projection, based on cam, also updates win depth

	Vector3			m_lightDir;				// the world renderer has a constant light dir per level
	Camera		  * m_realCam;
	TerrainRenderer  m_terrainRenderer;
	ParticleSystemsRenderer  m_psRenderer;
	ShipRenderer	 m_shipRenderer;
	LaserRenderer    m_laserRenderer;
	SpawnPointRenderer  m_spawnPointRenderer;
	//ClampRenderer	m_clampRenderer;
	//MiscFXRenderer	m_miscFXRenderer;

	bool			m_playerActive;
	const CoordinateFrame * m_playerCoordFrame;
	bool			m_boundsComputed;

	float m_currentTime;

	bool			m_postProcOn;
	bool			m_wireframeOn;

	// Matrices 
	double m_projd[16];
	double m_mviewd[16];
	float m_projf[16];
	float m_mviewf[16];
};


#endif
