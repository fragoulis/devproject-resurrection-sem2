//*****************************************************************************
//**
//**   File:               WorldRenderer.h
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-12 04:31:49 +0000 (Wed, 12 Mar 2008) $
//**   Last Revision:      $Revision: 151 $
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"
#include "TerrainRenderer.h"
#include "SpawnPointRenderer.h"
#include "ParticleSystemsRenderer.h"
#include "LaserRenderer.h"
#include "ShipRenderer.h"
#include "../Math/Vector3.h"
#include "../GameLogic/Objects/Playership.h"
class Camera;
class CoordinateFrame;

/**
 * Renders the world!
 *
 */



class WorldRenderer : public IRenderer,  
					  public EventListener< Player_Spawned >,
					  public EventListener< Player_Destroyed >
{
public:
	WorldRenderer();
	virtual ~WorldRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	const Vector3& getLightDir() const {return m_lightDir;}
	void setLightDir(const Vector3& ld) {m_lightDir.set(ld.getX(),ld.getY(),ld.getZ());}
	Camera * getCamera() {return m_camera;}
	void newCamera(Camera * cam);		// the argument must be new-d before passing


	// stored Matrices & stuff
	const double * getModelViewMatrixd() const {return m_mviewd;}
	const double * getProjectionMatrixd() const {return m_projd;}
	const float * getModelViewMatrixf() const {return m_mviewf;}
	const float * getProjectionMatrixf() const {return m_projf;}

	// Events
	void onEvent(Player_Spawned&);
	void onEvent(Player_Destroyed&);

private:

	void _updateMatrices();					// updates modelview & projection, based on cam, also updates win depth

	Vector3			m_lightDir;				// the world renderer has a constant light dir per level
	Camera		  * m_camera;				// The world renderer has our main camera
	TerrainRenderer m_terrainRenderer;
	ParticleSystemsRenderer m_psRenderer;
	ShipRenderer	m_shipRenderer;
	LaserRenderer   m_laserRenderer;
	SpawnPointRenderer m_spawnPointRenderer;

	bool			m_playerActive;
	const CoordinateFrame * m_playerCoordFrame;

	// Matrices 
	double m_projd[16];
	double m_mviewd[16];
	float m_projf[16];
	float m_mviewf[16];
};
