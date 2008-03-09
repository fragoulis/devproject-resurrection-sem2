//*****************************************************************************
//**
//**   File:               Plane.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "Vector3.h"
class Point3;
class Line;
class LineSegment;

class Plane
{
public:
	Plane();
	Plane(const Point3& point, const Vector3& normal);
	Plane(const Point3& p1, const Point3& p2, const Point3& p3);
	~Plane();

	const Vector3& getNormal() { return m_normal; }
	float getD() { return m_d; }

	bool onBackside(const Point3& point) const;
	float distance(const Point3& point) const;

	// returns false if parallel, vector has to be unit length
	bool intersectsLine(const Line& line, Point3& intersection_point) const;
	bool intersectsLine(const Point3& point, const Vector3& vector, Point3& intersection_point) const;

	bool intersectsLineSegment(const LineSegment& ls, Point3& intersection_point) const;
	bool intersectsLineSegment(const Point3& p1, const Point3& p2, Point3& intersection_point) const;

private:
	Vector3 m_normal;
	float m_d;
};
