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

	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	}

	glPopAttrib();
}

void PlanetRenderer :: update(float dt)
{
}
