#pragma once
#include <iosfwd>
#include "Vector2.h"

class Point2
{
public:
	Point2() { }
	Point2(const float components[]);
	Point2(float x, float y);
	~Point2();

	float getX() const { return m_vector.getX(); }
	float getY() const { return m_vector.getY(); }

	void setX(float x) { m_vector.setX(x); }
	void setY(float y) { m_vector.setY(y); }
	void set(float x, float y);

	void add(const Vector2& v);
	Vector2 subtract(const Point2& v) const;
	void subtract(const Vector2& v);
	float distance(const Point2& v) const;

	const float* cfp() const { return m_vector.cfp(); }

	std::istream& read(std::istream& is);
	std::ostream& write(std::ostream& os) const;

	Point2& operator += (const Vector2& rhs) { add(rhs); return *this; }
	Point2& operator -= (const Vector2& rhs) { subtract(rhs); return *this; }

	std::istream& operator >> (std::istream& is) { return read(is); }
	std::ostream& operator << (std::ostream& os) const { return write(os); }

private:
	Vector2 m_vector;
};








inline const Vector2 operator - (const Point2& lhs, const Point2& rhs)
{
	return lhs.subtract(rhs);
}

inline const Point2 operator + (const Point2& lhs, const Vector2& rhs)
{
	Point2 ret(lhs);
	ret += rhs;
	return ret;
}

inline const Point2 operator + (const Vector2& lhs, const Point2& rhs)
{
	Point2 ret(rhs);
	ret += lhs;
	return ret;
}

inline const Point2 operator - (const Point2& lhs, const Vector2& rhs)
{
	Point2 ret(lhs);
	ret -= rhs;
	return ret;
}

inline const Point2 operator - (const Vector2& lhs, const Point2& rhs)
{
	Point2 ret(rhs);
	ret -= lhs;
	return ret;
}
