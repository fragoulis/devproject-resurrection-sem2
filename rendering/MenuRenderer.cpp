//*****************************************************************************
//**
//**   File:               MenuRenderer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "MenuRenderer.h"
#include <gl/glee.h>

MenuRenderer::MenuRenderer()
{
}

MenuRenderer::~MenuRenderer()
{
}

void MenuRenderer :: render(Graphics& g) const
{
	// Render the menu here :D


	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0.8f, 0.5f, 0.55f, 1.0f);
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

void MenuRenderer :: update(float dt)
{
}
