//*****************************************************************************
//**
//**   File:               WorldRenderer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"
#include "TerrainRenderer.h"
#include "ParticleSystemsRenderer.h"
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


	// Events
	void onEvent(Player_Spawned&);
	void onEvent(Player_Destroyed&);

private:

	Vector3			m_lightDir;				// the world renderer has a constant light dir per level
	Camera		  * m_camera;				// The world renderer has our main camera
	TerrainRenderer m_terrainRenderer;
	ParticleSystemsRenderer m_psRenderer;
	ShipRenderer	m_shipRenderer;

	bool			m_playerActive;
	const CoordinateFrame * m_playerCoordFrame;
};
