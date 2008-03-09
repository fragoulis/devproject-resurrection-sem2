//*****************************************************************************
//**
//**   File:               Material.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Material.h"

Material::Material() {
	m_ambient = Color::black();
	m_diffuse = Color::white();
}

void Material::Set(const Color &ambient, const Color &diffuse, const Color &specular, const Color &emission, GLfloat shininess) {
	SetAmbient(ambient);
	SetDiffuse(diffuse);
	SetSpecular(specular);
	SetEmission(emission);
	SetShininess(shininess);
}

void Material::SetAmbient(const Color &ambient) {
	m_ambient = ambient;
}		

void Material::SetDiffuse(const Color &diffuse) {
	m_diffuse = diffuse;
}		

void Material::SetEmission(const Color &emission) {
	m_emission = emission;
}	

void Material::SetSpecular(const Color &specular) {
	m_specular = specular;
}

void Material::SetShininess(const GLfloat &shininess) {
	m_shininess = shininess;
}

void Material::Apply() const {
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient.rgba());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse.rgba());
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular.rgba());
	glMaterialfv(GL_FRONT, GL_EMISSION, m_emission.rgba());
	glMaterialf(GL_FRONT, GL_SHININESS, m_shininess);
}

Material &Material::operator=(const Material &material) {
	SetAmbient(material.GetAmbient());
	SetDiffuse(material.GetDiffuse());
	SetSpecular(material.GetSpecular());
	SetEmission(material.GetEmission());
	SetShininess(material.GetShininess());

	return *this;
}