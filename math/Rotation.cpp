//*****************************************************************************
//**
//**   File:               Rotation.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Rotation.h"
#include "Vector3.h"
#include "Point3.h"
#include "Matrix44.h"
#include "maths.h"
#include <cmath>

Rotation::Rotation()
{
	clear();
}

Rotation::~Rotation()
{
}


Rotation::Rotation(const Vector3& axis, const float angle)
{
	set(axis, angle);
}

Vector3 Rotation::getAxis() const
{
	if (Math::float_is_zero(m_q.getScalar() - 1.0f)) return Vector3(1.0f, 0.0f, 0.0f);
	return m_q.getVector() / sqrt(1 - m_q.getScalar() * m_q.getScalar());
}
float Rotation::getAngle() const
{
	if (Math::float_is_zero(m_q.getScalar() - 1.0f)) return 0.0f;
	return 2.0f * acos(m_q.getScalar());
}

Matrix44 Rotation::getMatrix() const
{
	float x = m_q.getX(), y = m_q.getY(), z = m_q.getZ(), w = m_q.getScalar();
	float xx = x * x * 2,
		  xy = x * y * 2,
		  xz = x * z * 2,
		  xw = x * w * 2,
		  yy = y * y * 2,
		  yz = y * z * 2,
		  yw = y * w * 2,
		  zz = z * z * 2,
		  zw = z * w * 2;

	return
		Matrix44(1 - yy - zz,  xy + zw,      xz - yw,      0.0f,
		         xy - zw,      1 - xx - zz,  yz + xw,      0.0f,
				 xz + yw,      yz - xw,      1 - xx - yy,  0.0f,
				 0.0f,         0.0f,         0.0f,         1.0f);
}
Matrix44 Rotation::getReverseMatrix() const
{
	float x = m_q.getX(), y = m_q.getY(), z = m_q.getZ(), w = m_q.getScalar();
	float xx = x * x * 2,
		  xy = x * y * 2,
		  xz = x * z * 2,
		  xw = x * w * 2,
		  yy = y * y * 2,
		  yz = y * z * 2,
		  yw = y * w * 2,
		  zz = z * z * 2,
		  zw = z * w * 2;

	return
		Matrix44(1 - yy - zz,  xy - zw,      xz + yw,      0.0f,
		         xy + zw,      1 - xx - zz,  yz - xw,      0.0f,
				 xz - yw,      yz + xw,      1 - xx - yy,  0.0f,
				 0.0f,         0.0f,         0.0f,         1.0f);
}


void Rotation :: clear()
{
	m_q.set(1.0f, 0.0f, 0.0f, 0.0f);
}

void Rotation :: set(const Vector3& axis, const float angle)
{
	m_q.set(cos(0.5f * angle), sin(0.5f * angle) * axis);
}

void Rotation :: set(float pitch, float yaw, float roll)
{
	float cosRoll = cos( roll * 0.5f );
	float cosPitch = cos( pitch * 0.5f );
	float cosYaw = cos( yaw * 0.5f );

	float sinRoll = sin( roll * 0.5f );
	float sinPitch = sin( pitch * 0.5f );
	float sinYaw = sin( yaw * 0.5f );

	float cpcy = cosPitch * cosYaw;
	float spsy = sinPitch * sinYaw;

	m_q.set(
		cosRoll * cpcy + sinRoll * spsy,
		sinRoll * cpcy - cosRoll * spsy,
		cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
		cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw
	);
	m_q.normalize();
}
	
void Rotation::rotate(const Vector3& axis, const float angle)
{
	m_q *= Quaternion(cos(0.5f * angle), sin(0.5f * angle) * axis);
}

void Rotation::rotate(const Vector3& angle)
{
	m_q += 0.5f * Quaternion(0.0f, angle) * m_q;
}

void Rotation::applyTo(Vector3& v) const
{
	v = (m_q * Quaternion(0.0f, v) * ~m_q).getVector();
}

void Rotation::applyTo(Point3& p) const
{
	p.set((m_q * Quaternion(0.0f, p.getVector()) * ~m_q).getVector());
}

void Rotation::applyReverseTo(Vector3& v) const
{
	v = (~m_q * Quaternion(0.0f, v) * m_q).getVector();
}

void Rotation::applyReverseTo(Point3& p) const
{
	p.set((~m_q * Quaternion(0.0f, p.getVector()) * m_q).getVector());
}
