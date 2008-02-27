#pragma once
#include "Point3.h"
class CoordinateFrame;

class LineSegment
{
public:
	LineSegment() { }
	LineSegment(const Point3& p1, const Point3& p2) : m_point1(p1), m_point2(p2) { }
	~LineSegment() { }

	const Point3& getPoint1() const { return m_point1; }
	const Point3& getPoint2() const { return m_point2; }
	Vector3 getVector() const { return m_point1 - m_point2; }

	void transform(const CoordinateFrame& cf);
	void reverseTransform(const CoordinateFrame& cf);

private:
	Point3 m_point1, m_point2;
};
