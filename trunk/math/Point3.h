#pragma once
#include <iosfwd>
#include "Vector3.h"

class Point3
{
public:
	Point3() { }
	Point3(const float components[]);
	Point3(float x, float y, float z);
	Point3(const Vector3& v);
	~Point3();

	float getX() const { return m_vector.getX(); }
	float getY() const { return m_vector.getY(); }
	float getZ() const { return m_vector.getZ(); }
	Vector3 getVector() const { return m_vector; }

	void setX(float x) { m_vector.setX(x); }
	void setY(float y) { m_vector.setY(y); }
	void setZ(float z) { m_vector.setZ(z); }
	void set(float x, float y, float z);
	void set(double x, double y, double z) { set(float(x), float(y), float(z)); }
	void set(int x, int y, int z) { set(float(x), float(y), float(z)); }
	void set(const Vector3& v);

	void add(const Vector3& v);
	Vector3 subtract(const Point3& v) const;
	void subtract(const Vector3& v);
	float distance(const Point3& v) const;

	static Vector3 calculateNormal(const Point3& p1, const Point3& p2, const Point3& p3);

	const float* cfp() const { return m_vector.cfp(); }

	std::istream& read(std::istream& is);
	std::ostream& write(std::ostream& os) const;

	Point3& operator += (const Vector3& rhs) { add(rhs); return *this; }
	Point3& operator -= (const Vector3& rhs) { subtract(rhs); return *this; }

private:
	Vector3 m_vector;
};








inline const Vector3 operator - (const Point3& lhs, const Point3& rhs)
{
	return lhs.subtract(rhs);
}

inline const Point3 operator + (const Point3& lhs, const Vector3& rhs)
{
	Point3 ret(lhs);
	ret += rhs;
	return ret;
}

inline const Point3 operator + (const Vector3& lhs, const Point3& rhs)
{
	Point3 ret(rhs);
	ret += lhs;
	return ret;
}

inline const Point3 operator - (const Point3& lhs, const Vector3& rhs)
{
	Point3 ret(lhs);
	ret -= rhs;
	return ret;
}

inline const Point3 operator - (const Vector3& lhs, const Point3& rhs)
{
	Point3 ret(rhs);
	ret -= lhs;
	return ret;
}

inline std::istream& operator >> (std::istream& is, Point3& p)
{
	return p.read(is);
}

inline std::ostream& operator << (std::ostream& os, const Point3& p)
{
	return p.write(os);
}
