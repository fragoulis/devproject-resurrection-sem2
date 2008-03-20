//*****************************************************************************
//**
//**   File:               Plane.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "Plane.h"
#include "maths.h"
#include "Point3.h"
#include "Line.h"
#include "LineSegment.h"


Plane::Plane()
{
}

Plane::~Plane()
{
}


Plane::Plane(const Point3& point, const Vector3& normal) : m_normal(normal), m_d(-normal.dot(point.getVector())) { }
Plane::Plane(const Point3& p1, const Point3& p2, const Point3& p3)
{
	m_normal = Point3::calculateNormal(p1, p2, p3);
	m_d = -m_normal.dot(p1.getVector());
}

bool Plane::onBackside(const Point3& point) const
{
	return (m_normal.dot(point.getVector()) + m_d) < 0.0f;
}

float Plane::distance(const Point3& point) const
{
	return fabs(m_normal.dot(point.getVector()) + m_d);
}


bool Plane::intersectsLine(const Line& line, Point3& intersection_point) const
{
	return intersectsLine(line.getPoint(), line.getVector(), intersection_point);
}
bool Plane::intersectsLine(const Point3& point, const Vector3& vector, Point3& intersection_point) const
{
	Vector3 v = vector;
	v.normalize();
	float ndotd = m_normal.dot(v);
	if (Math::float_is_zero(ndotd)) return false;
	float t = -( m_normal.dot(point.getVector()) + m_d ) / ( ndotd );
	intersection_point = point + v * t;
	return true;
}

bool Plane::intersectsLineSegment(const LineSegment& ls, Point3& intersection_point) const
{
	return intersectsLineSegment(ls.getPoint1(), ls.getPoint2(), intersection_point);
}

bool Plane::intersectsLineSegment(const Point3& p1, const Point3& p2, Point3& intersection_point) const
{
	if (onBackside(p1) != onBackside(p2)) return intersectsLine(p1, p1 - p2, intersection_point);
	return false;
}
