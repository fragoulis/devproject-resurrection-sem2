//*****************************************************************************
//**
//**   File:               Quaternion.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Quaternion.h"
#include <cmath>
#include <cassert>

Quaternion::Quaternion(float s, const Vector3& v)
{
	setScalar(s);
	setVector(v);
}

Quaternion::Quaternion(float s, float v1, float v2, float v3)
{
	setScalar(s);
	setVector(v1, v2, v3);
}


void Quaternion::setVector(float v1, float v2, float v3)
{
	m_vector.set(v1, v2, v3);
}

void Quaternion::add(const Quaternion& q)
{
	m_scalar += q.m_scalar;
	m_vector += q.m_vector;
}

void Quaternion::subtract(const Quaternion& q)
{
	m_scalar -= q.m_scalar;
	m_vector -= q.m_vector;
}

void Quaternion::multiply(const Quaternion& q)
{
	float new_scalar = m_scalar * q.m_scalar - m_vector.dot(q.m_vector);
	m_vector = m_scalar * q.m_vector + q.m_scalar * m_vector + Vector3::cross(m_vector, q.m_vector);
	m_scalar = new_scalar;
}

void Quaternion::multiply(float m)
{
	m_scalar *= m;
	m_vector *= m;
}

void Quaternion::conjugate()
{
	m_vector.negate();
}

void Quaternion::normalize()
{
	assert(squareLength() > 0.0f);
	multiply(1.0f / length());
}


float Quaternion :: squareLength() const
{
	return m_scalar * m_scalar + m_vector.dot(m_vector);
}

float Quaternion :: length() const
{
	return sqrt(squareLength());
}