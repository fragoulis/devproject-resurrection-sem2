#pragma once
#include "Vector3.h"
#include "Point3.h"
class CoordinateFrame;

class Line
{
public:
	Line() { }
	Line(const Point3& p1, const Point3& p2) : m_point(p1), m_vector(p1 - p2) { }
	Line(const Point3& p, const Vector3& v) : m_point(p), m_vector(v) { }
	~Line() { }

	const Point3 getPoint() const { return m_point; }
	const Vector3 getVector() const { return m_vector; }

	void transform(const CoordinateFrame& cf);
	void reverseTransform(const CoordinateFrame& cf);

private:
	Point3 m_point;
	Vector3 m_vector;
};
