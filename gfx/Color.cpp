//*****************************************************************************
//**
//**   File:               Color.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Color.h"

void Color::Set(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	m_rgba[0] = r;
	m_rgba[1] = g;
	m_rgba[2] = b;
	m_rgba[3] = a;
}

Color::Color() { 
	Set(0.0, 0.0, 0.0, 0.0);
}

Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	Set(r, g, b, a);
}

Color::Color(GLfloat rgba[]) {
	Set(rgba[0], rgba[1], rgba[2], rgba[3]);
}

const GLfloat *Color::rgba() const {
	return m_rgba;
}

Color Color::black() {
	return Color(0.0, 0.0, 0.0, 1.0);
}

Color Color::white() {
	return Color(1.0, 1.0, 1.0, 1.0);
}

Color Color::red() {
	return Color(1.0, 0.0, 0.0, 1.0);
}

Color Color::green() {
	return Color(0.0, 1.0, 0.0, 1.0);
}

Color Color::blue() {
	return Color(0.0, 0.0, 1.0, 1.0);
}

Color Color::yellow() {
	return Color(1.0, 1.0, 0.0, 1.0);
}

Color Color::null() {
	return Color(0.0, 0.0, 0.0, 0.0);
}
