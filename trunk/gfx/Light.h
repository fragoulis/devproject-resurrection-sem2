//*****************************************************************************
//**
//**   File:               Light.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once

#include "../Math/Vector4.h"
#include "Color.h"

//opengl light utility class
class Light {
private:
	GLuint m_number;	// light number
	Color m_ambient;
	Color m_diffuse;
	Color m_specular;

	Vector4 m_position;
	Vector4 m_spotDirection;
	GLfloat m_spotCutoff;
	GLfloat m_spotExponent;

	GLfloat m_constantAttenuation;
	GLfloat m_linearAttenuation;
	GLfloat m_quadraticAttenuation;

public:
	Light();

	Light &operator=(const Light &light);

	void Create(GLuint number, const Color &ambient=Color::black(), const Color &diffuse=Color::white(), const Color &specular=Color::white(), const Vector4 &position=Vector4(0.0f,0.0f,0.0f,1.0f));

	void SetAmbient(const Color &ambient);
	void SetDiffuse(const Color &diffuse);
	void SetSpecular(const Color &specular);
	void SetPosition(const Vector4 &position);
	void SetSpotDirection(const Vector4 &spotDirection);
	void SetSpotParams(const GLfloat &spotCutoff, const GLfloat &spotExponent);
	void SetAttenuations(const GLfloat &constantAttenuation, const GLfloat &linearAttenuation, const GLfloat &quadraticAttenuation);

	const Color &GetAmbient() const { return m_ambient; };
	const Color &GetDiffuse() const { return m_diffuse; };
	const Color &GetSpecular() const { return m_specular; };
	const Vector4 &GetPosition() const { return m_position; };
	const Vector4 &GetSpotDirection() const { return m_spotDirection; };
	const GLfloat &GetSpotCutOff() const { return m_spotCutoff; };
	const GLfloat &GetSpotExponent() const { return m_spotExponent; };
	const GLfloat &GetConstantAttenuation() const { return m_constantAttenuation; };
	const GLfloat &GetLinearAttenuation() const { return m_linearAttenuation; };
	const GLfloat &GetQuadraticAttenuation() const { return m_quadraticAttenuation; };

	void Apply() const;

	void Unapply() const;
};