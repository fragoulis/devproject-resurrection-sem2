//*****************************************************************************
//**
//**   File:               Color.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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