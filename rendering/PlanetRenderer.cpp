#include "PauseRenderer.h"
#include <gl/glee.h>

PauseRenderer::PauseRenderer()
{
}

PauseRenderer::~PauseRenderer()
{
}


void PauseRenderer :: render(Graphics& g) const
{
	// Render the menu here :D


	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.3f, 0.9f, 0.3f, 0.5f);
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

void PauseRenderer :: update(float dt)
{
}
