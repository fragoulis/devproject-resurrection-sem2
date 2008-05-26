#include "WorldRenderer.h"
#include "RenderEngine.h"
#include "../gfx/Camera.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include "RenderEngine.h"
#include "../GameLogic/GameLogic.h"
#include <string>
#include <vector>

#include "../Math/Matrix44.h"
#include "../gfx/MatrixTransform.h"

using namespace std;

WorldRenderer :: WorldRenderer()
:m_realCam(0),
m_playerActive(false),
m_boundsComputed(false),
m_currentTime(0.0f),
m_postProcOn(true),
m_wireframeOn(false)
{
	EventManager::instance().registerEventListener< Level_Unload >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Despawned >(this);
	EventManager::instance().registerEventListener< Level_Complete >(this);

	const float cam_addh = RenderEngine::instance().getCameraHeightAbovePlane();

	//m_terrainRenderer.setCamera(m_camera);

	m_realCam = new Camera();
	m_realCam->setPerspective(30, 1.0f, cam_addh - 100.0f, cam_addh + 2000.0f);

	//m_terrainRenderer.setShipRendererRef(&m_shipRenderer);
	//m_terrainRenderer.setLaserRendererRef(&m_laserRenderer);
}

void WorldRenderer :: adjustCameras()
{
	m_boundsComputed = false;
	delete m_realCam;

	const float cam_addh = RenderEngine::instance().getCameraHeightAbovePlane();
	int vp[4];
	RenderEngine::instance().getViewport(vp);

	m_realCam = new Camera();
	m_realCam->setPerspective(30, 0.75f*float(vp[2] / float(vp[3])), cam_addh - 100.0f, cam_addh + 2000.0f);
}

WorldRenderer :: ~WorldRenderer()
{
	if(m_realCam)
	{
		delete m_realCam;
		m_realCam = 0;
	}

	EventManager::instance().unRegisterEventListener< Level_Unload >(this);
	EventManager::instance().unRegisterEventListener< Player_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Player_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Level_Complete >(this);
}

void WorldRenderer :: onEvent(Level_Unload& evt)
{
	m_playerActive = false;
	m_boundsComputed = false;
	m_playerCoordFrame = 0;
}

void WorldRenderer :: onEvent(Level_Complete& evt)
{
	m_playerActive = false;
}


void WorldRenderer :: render(Graphics& g) const
{
	RenderEngine::POLY_COUNT = 0;
	// now render opaque 3D stuff

	//m_terrainRenderer.renderShadows();
	//m_terrainRenderer.updateTerraformContribution();
	//m_terrainRenderer.drawLakeReflection(g);
	

	// ENABLE DEPTH TEST HERE! ALSO RESET ANY STATUS HERE!

	// set modelview to perspective cam. Swaps projection AND modelview (mv to 0, 2d uses 1)
	Camera::activatePerspective();
	
	m_shipRenderer.render(g);
	m_terrainRenderer.render(g);
	m_spawnPointRenderer.render(g);
	m_laserRenderer.render(g);
	m_psRenderer.render(g);
	//m_miscFXRenderer.render(g);
	//m_clampRenderer.render(g);

	//OSReport("Triangles rendered : %u\n",RenderEngine::POLY_COUNT);
}

void WorldRenderer :: update( float dt )
{	
	m_currentTime += dt;
	// Yes, this is a hack
	if(!m_boundsComputed)
	{
		computeBounds();
		m_boundsComputed = true;
	}

	if (m_playerActive)
	{
		const Vector3 playerpos(m_playerCoordFrame->getOrigin().cfp());
		Vector3 camera_eye(playerpos + Vector3(0.0f,RenderEngine::instance().getCameraHeightAbovePlane(),0.0f));
		Vector3 camera_look(playerpos);

		//RenderEngine::instance().boundCameraPosition(camera_eye);
		//RenderEngine::instance().boundCameraPosition(camera_look);

		MatrixTransform::LoadIdentity();

		m_realCam->setPosition(camera_eye,
							  camera_look,
							  Vector3(0.0f,0.0f,-1.0f));
		m_realCam->slide(0.0f,-tanf(30.0f*PI / 180.0)*RenderEngine::instance().getCameraHeightAbovePlane(),0.0f);
		m_realCam->pitch(30.0f);

		MatrixTransform::MulMatrix(Camera::viewMatrix);


		//
		//_updateMatrices();
		////RenderEngine::instance().computeWsScreenEdges();
		
	}

	m_realCam->update(dt);
	m_psRenderer.update(dt);
	m_spawnPointRenderer.update(dt);
	m_terrainRenderer.update(dt);
	m_laserRenderer.update(dt);
	//m_clampRenderer.update(dt);
	//m_miscFXRenderer.update(dt);
	//m_shipRenderer.update(dt);

}

void WorldRenderer :: newCamera(Camera * cam)
{
	if(m_realCam)
		delete m_realCam;
	m_realCam = cam;
}

void WorldRenderer :: onEvent(Player_Spawned& evt)
{
	m_playerActive = true;
	m_playerCoordFrame = &(evt.getValue()->getCoordinateFrame());
}

void WorldRenderer :: onEvent(Player_Despawned&)
{
	m_playerActive = false;
}


void WorldRenderer :: _updateMatrices()
{
	//glGetFloatv(GL_MODELVIEW_MATRIX,m_mviewf);
	//glGetFloatv(GL_PROJECTION_MATRIX,m_projf);
	//for(int i=0;i<16;++i)
	//{
	//	m_mviewd[i] = double(m_mviewf[i]);
	//	m_projd[i] = double(m_projf[i]);
	//}
}

void WorldRenderer :: computeBounds()
{
	//// compute the bounds for camera and player.

	//// set a dummy camera	
	//glPushAttrib(GL_MATRIX_MODE);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//m_realCam->setPosition(Vector3(0.0f,GameLogic::instance().getGamePlaneHeight()+RenderEngine::instance().getCameraHeightAbovePlane(),0.0f),
	//						  Vector3(0.0f,0.0f,0.0f),
	//						  Vector3(0.0f,0.0f,-1.0f));
	//m_realCam->slide(0.0f,-tanf(30.0f*math_const<float>::DEG2RAD)*RenderEngine::instance().getCameraHeightAbovePlane(),0.0f);
	//m_realCam->pitch(30.0f);
	//_updateMatrices();

	//// get the 4 extents
	//Point3 edges[4];
	//RenderEngine::instance().getWsScreenEdges(edges,0.0f);

	//Point3 playeredges[4];
	//RenderEngine::instance().getWsScreenEdges(playeredges,GameLogic::instance().getGamePlaneHeight());

	//// get the map extents
	//Vector3 ext(m_terrainRenderer.getMapExtents());

	//// move our pos so that ll edge is ll of the map
	//Vector3 ll_mapedge(-ext.getX()*0.5f,0.0f,ext.getZ()*0.5f);
	//Vector3 to_ll_edge_vector = (ll_mapedge - edges[0]).getVector();
	//Point3 ll_topleftedge(edges[3] + to_ll_edge_vector);

	//// we compute the negative x boundary. The positive will be inverse.
	//// this boundary is the player's boundary.
	//float pl_xbound = -2.0f*ext.getX()*0.5f - ll_topleftedge.getX();
	//float cam_xbound = pl_xbound + (-edges[0].getX());

	//// move our pos so that ll edge is ll of the map
	//float u_map_edge = -ext.getZ()*0.5f;
	//float b_map_edge = ext.getZ()*0.5f;
	//float cam_zbound = u_map_edge - edges[2].getZ();

	//// below : actually I should use the plane at the max height of the map,get the edges, 
	//// & use one of the lower edges, but the difference is minimal
	//float cam_lowzbound = b_map_edge - playeredges[1].getZ();	

	//float pl_zbound = u_map_edge - edges[2].getZ() + playeredges[2].getZ();

	//RenderEngine::instance().setPlayerBounds(Vector4(pl_xbound,-pl_xbound,ext.getZ()*0.5f,pl_zbound));
	//RenderEngine::instance().setCameraBounds(Vector4(cam_xbound,-cam_xbound,cam_lowzbound,cam_zbound));

	//glPopMatrix();
	//glPopAttrib();
}