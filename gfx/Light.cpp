//*****************************************************************************
//**
//**   File:               Light.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "Light.h"

Light::Light() {
	m_number = -1;
}

Light &Light::operator=(const Light &light) {
	SetAmbient(light.GetAmbient());
	SetDiffuse(light.GetDiffuse());
	SetSpecular(light.GetSpecular());
	SetPosition(light.GetPosition());
	SetSpotDirection(light.GetSpotDirection());
	SetSpotParams(light.GetSpotCutOff(), light.GetSpotExponent());
	SetAttenuations(light.GetConstantAttenuation(), light.GetLinearAttenuation(),light.GetQuadraticAttenuation());

	return *this;
}

void Light::Create(GLuint number, const Color &ambient, const Color &diffuse, const Color &specular, const Vector4 &position) {
	m_number = number;	

	SetAmbient(ambient);
	SetDiffuse(diffuse);
	SetSpecular(specular);
	SetPosition(position);
}

void Light::SetAmbient(const Color &ambient) {
	m_ambient = ambient;

	glLightfv(GL_LIGHT0+m_number, GL_AMBIENT, m_ambient.rgba());
}		

void Light::SetDiffuse(const Color &diffuse) {
	m_diffuse = diffuse;

	glLightfv(GL_LIGHT0+m_number, GL_DIFFUSE, m_diffuse.rgba());
}		

void Light::SetSpecular(const Color &specular) {
	m_specular = specular;

	glLightfv(GL_LIGHT0+m_number, GL_SPECULAR, m_specular.rgba());
}

void Light::SetPosition(const Vector4 &position) {
	m_position = position;

	glLightfv(GL_LIGHT0+m_number, GL_POSITION, m_position.cfp());
}

void Light::SetSpotDirection(const Vector4 &spotDirection) {
	m_spotDirection = spotDirection;

	glLightfv(GL_LIGHT0+m_number, GL_SPOT_DIRECTION, m_spotDirection.cfp());
}

void Light::SetSpotParams(const GLfloat &spotCutoff, const GLfloat &spotExponent) {
	m_spotCutoff = spotCutoff;
	m_spotExponent = spotExponent;

	glLightf(GL_LIGHT0+m_number,GL_SPOT_CUTOFF, m_spotCutoff);
	glLightf(GL_LIGHT0+m_number,GL_SPOT_EXPONENT, m_spotExponent);
}

void Light::SetAttenuations(const GLfloat &constantAttenuation, const GLfloat &linearAttenuation, const GLfloat &quadraticAttenuation) {
	m_constantAttenuation = constantAttenuation;
	m_linearAttenuation = linearAttenuation;
	m_quadraticAttenuation = quadraticAttenuation;

	glLightf(GL_LIGHT0+m_number,GL_CONSTANT_ATTENUATION, m_constantAttenuation);
	glLightf(GL_LIGHT0+m_number,GL_LINEAR_ATTENUATION, m_linearAttenuation);
	glLightf(GL_LIGHT0+m_number,GL_QUADRATIC_ATTENUATION, m_quadraticAttenuation);
}

void Light::Apply() const {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0 + m_number);
}

void Light::Unapply() const {
	glDisable(GL_LIGHT0 + m_number);
}