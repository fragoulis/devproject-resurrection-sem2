#pragma once
#include <iosfwd>
class Matrix33;

class Vector3
{
public:
	Vector3() { }
	Vector3(const float components[]);
	Vector3(float x, float y, float z);
	~Vector3();

	float getX() const { return m_components[0]; }
	float getY() const { return m_components[1]; }
	float getZ() const { return m_components[2]; }

	void setX(float x) { m_components[0] = x; }
	void setY(float y) { m_components[1] = y; }
	void setZ(float z) { m_components[2] = z; }
	void set(float x, float y, float z);
	void set(double x, double y, double z) { set(float(x), float(y), float(z)); }
	void set(int x, int y, int z) { set(float(x), float(y), float(z)); }

	void add(const Vector3& v);
	void add(float x, float y, float z);
	void subtract(const Vector3& v);
	void subtract(float x, float y, float z);
	void multiply(float m);
	void multiply(float x, float y, float z);
	void multiply(const Matrix33& m);
	void divide(float m) { multiply(1.0f / m); }
	void negate();
	void normalize();
	float length() const;
	float dot(const Vector3& v) const;
	float angle(const Vector3& v) const; //assumes both are unit length
	void cross(const Vector3& rhs);

	const float* cfp() const { return m_components; }

	std::istream& read(std::istream& is);
	std::ostream& write(std::ostream& os) const;


	Vector3& operator += (const Vector3& rhs) { add(rhs); return *this; }
	Vector3& operator -= (const Vector3& rhs) { subtract(rhs); return *this; }
	Vector3& operator *= (float rhs) { multiply(rhs); return *this; }
	Vector3& operator /= (float rhs) { divide(rhs); return *this; }
	Vector3& operator *= (const Matrix33& rhs) { multiply(rhs); return *this; }

	static const Vector3 cross(const Vector3& lhs, const Vector3& rhs);

private:
	float m_components[3];
};








inline const Vector3 operator + (const Vector3& lhs, const Vector3& rhs)
{
	Vector3 ret(lhs);
	ret += rhs;
	return ret;
}

inline const Vector3 operator - (const Vector3& lhs, const Vector3& rhs)
{
	Vector3 ret(lhs);
	ret -= rhs;
	return ret;
}

inline const Vector3 operator * (const Vector3& lhs, const float rhs)
{
	Vector3 ret(lhs);
	ret *= rhs;
	return ret;
}

inline const Vector3 operator * (const float lhs, const Vector3& rhs)
{
	Vector3 ret(rhs);
	ret *= lhs;
	return ret;
}

inline const Vector3 operator / (const Vector3& lhs, const float rhs)
{
	Vector3 ret(lhs);
	ret /= rhs;
	return ret;
}

inline const Vector3 operator / (const float lhs, const Vector3& rhs)
{
	Vector3 ret(rhs);
	ret /= lhs;
	return ret;
}

inline const Vector3 operator - (const Vector3& rhs) {
	Vector3 ret(rhs);
	ret.negate();
	return ret;
}


inline std::istream& operator >> (std::istream& is, Vector3& v)
{
	return v.read(is);
}

inline std::ostream& operator << (std::ostream& os, const Vector3& v)
{
	return v.write(os);
}
