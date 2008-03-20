//*****************************************************************************
//**
//**   File:               Point3.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "Point3.h"
#include <cmath>
#include <iostream>
using namespace std;

Point3::Point3(const float components[])
{
	set(components[0], components[1], components[2]);
}
Point3::Point3(const float x, const float y, const float z)
{
	set(x, y, z);
}
Point3::Point3(const Vector3& v)
{
	set(v);
}

Point3::~Point3()
{
}

void Point3::set(const float x, const float y, const float z)
{
	setX(x);
	setY(y);
	setZ(z);
}
void Point3::set(const Vector3& v)
{
	m_vector = v;
}

void Point3::add(const Vector3& v)
{
	m_vector.add(v);
}

Vector3 Point3::subtract(const Point3& p) const
{
	return m_vector - p.m_vector;
}
void Point3::subtract(const Vector3& v)
{
	m_vector -= v;
}

float Point3::distance(const Point3& p) const
{
	return float(sqrt(fabs(
		(getX() - p.getX()) * (getX() - p.getX()) +
		(getY() - p.getY()) * (getY() - p.getY()) +
		(getZ() - p.getZ()) * (getZ() - p.getZ())
		)));
}


Vector3 Point3::calculateNormal(const Point3& p1, const Point3& p2, const Point3& p3)
{
	Vector3 normal = Vector3::cross(p1 - p2, p2 - p3);
	normal.normalize();
	return normal;
}



istream& Point3::read(istream& is)
{
	return m_vector.read(is);
}
ostream& Point3::write(ostream& os) const
{
	return m_vector.write(os);
}
