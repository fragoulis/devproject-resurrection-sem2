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

void Quaternion :: slerp( const Quaternion& target, float time )
{
    float to1[4];

    // calc cosine
    float cosom = getX() * target.getX() +
                  getY() * target.getY() +
                  getZ() * target.getZ() +
                  getScalar() * target.getScalar();

    // adjust signs (if necessary)
    if( cosom < 1e-6f ) {
        cosom = -cosom;
        to1[0] = -target.getX();
        to1[1] = -target.getY();
        to1[2] = -target.getZ();
        to1[3] = -target.getScalar();
    } else  {
        to1[0] = target.getX();
        to1[1] = target.getY();
        to1[2] = target.getZ();
        to1[3] = target.getScalar();
    }

    // calculate coefficients
    float  omega, sinom, scale0, scale1;
    if( ( 1.0f - cosom ) > 1e-3f ) {
        // standard case (slerp)
        omega   = acos(cosom);
        sinom   = 1.0f / sin(omega);
        scale0  = sin( ( 1.0f - time ) * omega ) * sinom;
        scale1  = sin( time * omega ) * sinom;
    } else {        
    // "from" and "to" quaternions are very close
    //  ... so we can do a linear interpolation
            scale0 = 1.0f - time;
            scale1 = time;
    }

    float rx = scale0 * getX() + scale1 * to1[0];
    float ry = scale0 * getY() + scale1 * to1[1];
    float rz = scale0 * getZ() + scale1 * to1[2];
    float rw = scale0 * getScalar() + scale1 * to1[3];

	set(rw, rx, ry, rz);
}