//*****************************************************************************
//**
//**   File:               Color.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include <gl/glee.h>

//opengl color utility class
class Color {
private:
	GLfloat m_rgba[4];

public:

	Color();
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	Color(GLfloat rgba[]);

	const GLfloat* rgba() const;
	void Set(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	static Color black();
	static Color white();
	static Color red();
	static Color green();
	static Color blue();
	static Color yellow();
	static Color null();
};