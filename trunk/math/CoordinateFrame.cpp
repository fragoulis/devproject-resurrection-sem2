//*****************************************************************************
//**
//**   File:               CoordinateFrame.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "CoordinateFrame.h"

CoordinateFrame::CoordinateFrame()
{
	m_origin.set(0.0f, 0.0f, 0.0f);
	m_rotation.clear();
	m_matrix.loadIdentity();
}

CoordinateFrame::~CoordinateFrame()
{
}


Matrix44 CoordinateFrame::getMatrix() const
{
	return m_matrix;
}

Matrix44 CoordinateFrame::getReverseMatrix() const
{
	return
		m_rotation.getMatrix() *
		Matrix44(1.0f, 0.0f, 0.0f, -m_origin.getX(),
		         0.0f, 1.0f, 0.0f, -m_origin.getY(),
				 0.0f, 0.0f, 1.0f, -m_origin.getZ(),
				 0.0f, 0.0f, 0.0f, 1.0f);
}


void CoordinateFrame::move(const Vector3& v)
{
	m_origin += v;
	_updateMatrix();
}

void CoordinateFrame::rotate(const Vector3& axis, float angle)
{
	m_rotation.rotate(axis, angle);
	_updateMatrix();
}

void CoordinateFrame::rotate(const Vector3& angle)
{
	m_rotation.rotate(angle);
	_updateMatrix();
}


void CoordinateFrame::transform(Vector3& v) const
{
	m_rotation.applyTo(v);
}

void CoordinateFrame::transform(Point3& p) const
{
	p -= m_origin.getVector();
	m_rotation.applyTo(p);
}

void CoordinateFrame::reverseTransform(Vector3& v) const
{
	m_rotation.applyReverseTo(v);
}

void CoordinateFrame::reverseTransform(Point3& p) const
{
	m_rotation.applyReverseTo(p);
	p += m_origin.getVector();
}

void CoordinateFrame :: _updateMatrix()
{
	m_matrix = //m_rotation.getMatrix() *
						Matrix44(1.0f, 0.0f, 0.0f, m_origin.getX(),
								 0.0f, 1.0f, 0.0f, m_origin.getY(),
								 0.0f, 0.0f, 1.0f, m_origin.getZ(),
								 0.0f, 0.0f, 0.0f, 1.0f) * m_rotation.getMatrix();
}