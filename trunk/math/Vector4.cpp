//*****************************************************************************
//**
//**   File:               Vector4.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Vector4.h"
#include "Matrix44.h" 
#include <cmath>
#include <iostream>
using namespace std;

Vector4::Vector4(const float components[])
{
	set(components[0], components[1], components[2], components[3]);
}
Vector4::Vector4(const float x, const float y, const float z, const float w)
{
	set(x, y, z, w);
}

Vector4::~Vector4()
{
}

void Vector4::set(const float x, const float y, const float z, const float w)
{
	setX(x);
	setY(y);
	setZ(z);
	setW(w);
}

void Vector4::add(const Vector4& v)
{
	add(v.getX(), v.getY(), v.getZ(), v.getW());
}
void Vector4::add(const float x, const float y, const float z, const float w)
{
	m_components[0] += x;
	m_components[1] += y;
	m_components[2] += z;
	m_components[3] += w;
}

void Vector4::subtract(const Vector4& v)
{
	subtract(v.getX(), v.getY(), v.getZ(), v.getW());
}

void Vector4::subtract(const float x, const float y, const float z, const float w)
{
	m_components[0] -= x;
	m_components[1] -= y;
	m_components[2] -= z;
	m_components[3] -= w;
}

void Vector4::multiply(const float m)
{
	multiply(m, m, m, m);
}

void Vector4::multiply(const float x, const float y, const float z, const float w)
{
	m_components[0] *= x;
	m_components[1] *= y;
	m_components[2] *= z;
	m_components[3] *= w;
}

void Vector4::multiply(const Matrix44& m)
{
	float x = m_components[0], y = m_components[1], z = m_components[2], w = m_components[3];
	setX(x * m.get(0, 0) + y * m.get(0, 1) + z * m.get(0, 2) + w * m.get(0, 3));
	setY(x * m.get(1, 0) + y * m.get(1, 1) + z * m.get(1, 2) + w * m.get(1, 3));
	setZ(x * m.get(2, 0) + y * m.get(2, 1) + z * m.get(2, 2) + w * m.get(2, 3));
	setW(x * m.get(3, 0) + y * m.get(3, 1) + z * m.get(3, 2) + w * m.get(3, 3));
}

void Vector4::negate()
{
	setX(-getX());
	setY(-getY());
	setZ(-getZ());
	setW(-getW());
}

float Vector4::length() const
{
	return float(sqrt(dot(*this)));
}

void Vector4::normalize()
{
	multiply(float(1.0 / length()));
}

float Vector4::dot(const Vector4& v) const
{
	return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ() + getW() * v.getW();
}



istream& Vector4::read(istream& is)
{
	char skip;
	ws(is);
	is >> skip >> m_components[0] >> skip >> m_components[1] >> skip >> m_components[2] >> skip >> m_components[3] >> skip;
	ws(is);
	return is;
}
ostream& Vector4::write(ostream& os) const
{
	os << '(' << m_components[0] << ", " << m_components[1] << ", " << m_components[2] << ", " << m_components[3] << ')';
	return os;
}
