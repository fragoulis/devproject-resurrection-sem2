//*****************************************************************************
//**
//**   File:               Graphics.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "Graphics.h"





//-------------------------------------------------------------------------------------------------------

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

//void Graphics::multCoordinateFrame(const CoordinateFrame& cf)
//{
//	glMultMatrixf(cf.getReverseMatrix().cfp());
//}

//-------------------------------------------------------------------------------------------------------


















//-------------------------------------------------------------------------------------------------------

//void Graphics::setMaterial(const Material& m)
//{
//	glMaterialfv(GL_FRONT, GL_AMBIENT, m.getAmbient().cfp());
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, m.getDiffuse().cfp());
//	glMaterialfv(GL_FRONT, GL_EMISSION, m.getEmission().cfp());
//	glMaterialfv(GL_FRONT, GL_SPECULAR, m.getSpecular().cfp());
//	glMaterialf(GL_FRONT, GL_SHININESS, m.getSpecularExp());
//}
//
//
//void Graphics::enableLighting()
//{
//	glEnable(GL_LIGHTING);
//	glShadeModel(GL_SMOOTH);
//}
//
//int Graphics::pushLight(const Light &light)
//{
//	glLightfv(GL_LIGHT0 + m_lastLightIndex, GL_POSITION, light.getPosition().cfp());
//	glLightfv(GL_LIGHT0 + m_lastLightIndex, GL_AMBIENT, light.getAmbient().cfp());
//	glLightfv(GL_LIGHT0 + m_lastLightIndex, GL_DIFFUSE, light.getDiffuse().cfp());
//	glLightfv(GL_LIGHT0 + m_lastLightIndex, GL_SPECULAR, light.getSpecular().cfp());
//	glLightfv(GL_LIGHT0 + m_lastLightIndex, GL_SPOT_DIRECTION, light.getSpotDirection().cfp());
//	glLightf(GL_LIGHT0 + m_lastLightIndex, GL_SPOT_EXPONENT, light.getSpotExponent());
//	glLightf(GL_LIGHT0 + m_lastLightIndex, GL_SPOT_CUTOFF, light.getSpotCutOff());
//	glLightf(GL_LIGHT0 + m_lastLightIndex, GL_CONSTANT_ATTENUATION, light.getConstantAttenuation());
//	glLightf(GL_LIGHT0 + m_lastLightIndex, GL_LINEAR_ATTENUATION, light.getLinearAttenuation());
//	glLightf(GL_LIGHT0 + m_lastLightIndex, GL_QUADRATIC_ATTENUATION, light.getQuadraticAttenuation());
//	enableLight(m_lastLightIndex);
//	return m_lastLightIndex++;
//}
//
//
//void Graphics::popLight()
//{
//	assert(m_lastLightIndex > 0);
//	disableLight(--m_lastLightIndex);
//}
//
//void Graphics::resetLights()
//{
//	for (int i = 0; i < MAX_LIGHTS; i++) disableLight(i);
//	m_lastLightIndex = 0;
//}
//
//void Graphics::setGlobalAmbient(const Color &c)
//{
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, c.cfp());
//}

//-------------------------------------------------------------------------------------------------------
