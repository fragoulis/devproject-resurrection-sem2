#include "WorldRenderer.h"
#include "../gfx/Camera.h"

WorldRenderer :: WorldRenderer()
:m_camera(0)
{
	m_camera = new Camera();
	m_camera->setPerspective(30, 1.0f, 10.0f, 10000.0f);
	m_camera->setPosition(Vector3(64, 300, -64), Vector3(64, 0, -64), Vector3(0,0,-1));
}

WorldRenderer :: ~WorldRenderer()
{
	if(m_camera)
		delete m_camera;
}


void WorldRenderer :: render(Graphics& g) const
{
	m_terrainRenderer.render(g);
	m_psRenderer.render(g);
	m_shipRenderer.render(g);
}

void WorldRenderer :: update( float dt )
{
	m_camera->update(dt);
	m_psRenderer.update(dt);
	//m_shipRenderer.update(dt);
}

void WorldRenderer :: newCamera(Camera * cam)
{
	if(m_camera)
		delete m_camera;
	m_camera = cam;
}