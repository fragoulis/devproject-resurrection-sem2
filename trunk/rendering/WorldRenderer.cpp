#include "WorldRenderer.h"
#include "../gfx/Camera.h"

WorldRenderer :: WorldRenderer()
:m_camera(0)
{
	m_camera = new Camera();
	m_camera->setPerspective(30, 1.0f, 10.0f, 10000.0f);
	m_camera->setPosition(Vector3(200, 800, -200), Vector3(200, 0, -200), Vector3(0,0,-1));
}

WorldRenderer :: ~WorldRenderer()
{
}


void WorldRenderer :: render(Graphics& g) const
{
	m_terrainRenderer.render(g);
}

void WorldRenderer :: update( float dt )
{
}

void WorldRenderer :: newCamera(Camera *& cam)
{
	if(m_camera)
		delete m_camera;
	m_camera = cam;
}