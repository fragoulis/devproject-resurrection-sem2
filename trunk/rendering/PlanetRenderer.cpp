#include "PlanetRenderer.h"
#include <gl/glee.h>

PlanetRenderer::PlanetRenderer()
{
}

PlanetRenderer::~PlanetRenderer()
{
}


void PlanetRenderer :: render(Graphics& g) const
{
	// Render the menu here :D


	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.3f, 0.5f, 0.9f, 1.0f);
	glBegin(GL_QUADS);
	{
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
}

void PlanetRenderer :: update(float dt)
{
}
