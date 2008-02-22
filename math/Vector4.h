#pragma once
#include "../utility/ISerializable.h"
class Matrix44;

class Vector4 : public ISerializable
{
public:
	Vector4() { }
	Vector4(const float components[]);
	Vector4(float x, float y, float z, float w);
	~Vector4();

	float getX() const { return m_components[0]; }
	float getY() const { return m_components[1]; }
	float getZ() const { return m_components[2]; }
	float getW() const { return m_components[3]; }

	void setX(float x) { m_components[0] = x; }
	void setY(float y) { m_components[1] = y; }
	void setZ(float z) { m_components[2] = z; }
	void setW(float w) { m_components[3] = w; }
	void set(float x, float y, float z, float w);
	void set(double x, double y, double z, double w) { set(float(x), float(y), float(z), float(w)); }
	void set(int x, int y, int z, int w) { set(float(x), float(y), float(z), float(w)); }

	void add(const Vector4& v);
	void add(float x, float y, float z, float w);
	void subtract(const Vector4& v);
	void subtract(float x, float y, float z, float w);
	void multiply(float m);
	void multiply(float x, float y, float z, float w);
	void multiply(const Matrix44& m);
	void negate();
	void normalize();
	float length() const;
	float dot(const Vector4& v) const;

	const float* cfp() const { return m_components; }

	virtual std::istream& read(std::istream& is);
	virtual std::ostream& write(std::ostream& os) const;


	Vector4& operator += (const Vector4& rhs) { add(rhs); return *this; }
	Vector4& operator -= (const Vector4& rhs) { subtract(rhs); return *this; }
	Vector4& operator *= (float rhs) { multiply(rhs); return *this; }
	Vector4& operator *= (const Matrix44& rhs) { multiply(rhs); return *this; }

private:
	float m_components[4];
};








inline const Vector4 operator + (const Vector4& lhs, const Vector4& rhs)
{
	Vector4 ret(lhs);
	ret += rhs;
	return ret;
}

inline const Vector4 operator - (const Vector4& lhs, const Vector4& rhs)
{
	Vector4 ret(lhs);
	ret -= rhs;
	return ret;
}

inline const Vector4 operator * (const Vector4& lhs, const float rhs)
{
	Vector4 ret(lhs);
	ret *= rhs;
	return ret;
}

inline const Vector4 operator * (const float lhs, const Vector4& rhs)
{
	Vector4 ret(rhs);
	ret *= lhs;
	return ret;
}

inline const Vector4 operator - (const Vector4& rhs) {
	Vector4 ret(rhs);
	ret.negate();
	return ret;
}
