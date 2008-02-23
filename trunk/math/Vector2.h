#pragma once
#include <iosfwd>
class Matrix22;

class Vector2
{
public:
	Vector2() { }
	Vector2(const float components[]);
	Vector2(float x, float y);
	~Vector2();

	float getX() const { return m_components[0]; }
	float getY() const { return m_components[1]; }

	void setX(float x) { m_components[0] = x; }
	void setY(float y) { m_components[1] = y; }
	void set(float x, float y);
	void set(double x, double y) { set(float(x), float(y)); }
	void set(int x, int y) { set(float(x), float(y)); }

	void add(const Vector2& v);
	void add(float x, float y);
	void subtract(const Vector2& v);
	void subtract(float x, float y);
	void multiply(float m);
	void multiply(float x, float y);
	void negate();
	void normalize();
	float length() const;
	float dot(const Vector2& v) const;
	float angle(const Vector2& v) const; //assumes both are unit length

	const float* cfp() const { return m_components; }

	std::istream& read(std::istream& is);
	std::ostream& write(std::ostream& os) const;

	Vector2& operator += (const Vector2& rhs) { add(rhs); return *this; }
	Vector2& operator -= (const Vector2& rhs) { subtract(rhs); return *this; }
	Vector2& operator *= (float rhs) { multiply(rhs); return *this; }

	std::istream& operator >> (std::istream& is) { return read(is); }
	std::ostream& operator << (std::ostream& os) const { return write(os); }

private:
	float m_components[2];
};








inline const Vector2 operator + (const Vector2& lhs, const Vector2& rhs)
{
	Vector2 ret(lhs);
	ret += rhs;
	return ret;
}

inline const Vector2 operator - (const Vector2& lhs, const Vector2& rhs)
{
	Vector2 ret(lhs);
	ret -= rhs;
	return ret;
}

inline const Vector2 operator * (const Vector2& lhs, const float rhs)
{
	Vector2 ret(lhs);
	ret *= rhs;
	return ret;
}

inline const Vector2 operator * (const float lhs, const Vector2& rhs)
{
	Vector2 ret(rhs);
	ret *= lhs;
	return ret;
}

inline const Vector2 operator - (const Vector2& rhs) {
	Vector2 ret(rhs);
	ret.negate();
	return ret;
}
