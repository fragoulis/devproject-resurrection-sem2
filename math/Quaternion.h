#ifndef _RES_QUATERNION_H_
#define _RES_QUATERNION_H_
#include "Vector3.h"

class tQuaternion
{
public:
	explicit tQuaternion() { }
	tQuaternion(float s, const Vector3& v);
	tQuaternion(float s, float v1, float v2, float v3);
	~tQuaternion() { }

	float getScalar() const { return m_scalar; }
	Vector3 getVector() const { return m_vector; }
	float getX() const { return m_vector.getX(); }
	float getY() const { return m_vector.getY(); }
	float getZ() const { return m_vector.getZ(); }

	void setScalar(float s) { m_scalar = s; }
	void setVector(const Vector3& v) { m_vector = v; }
	void setVector(float v1, float v2, float v3);
	void set(float s, const Vector3& v) { setScalar(s); setVector(v); }
	void set(float s, float v1, float v2, float v3) { setScalar(s); setVector(v1, v2, v3); }

	void add(const tQuaternion& q);
	void subtract(const tQuaternion& q);
	void multiply(const tQuaternion& q); // Grassmann
	void multiply(float m);
	void conjugate();
	void normalize();
	void slerp(const tQuaternion& target, float time);

	float squareLength() const;
	float length() const;


	tQuaternion& operator += (const tQuaternion& rhs) { add(rhs); return *this; }
	tQuaternion& operator -= (const tQuaternion& rhs) { subtract(rhs); return *this; }
	tQuaternion& operator *= (const tQuaternion& rhs) { multiply(rhs); return *this; }
	tQuaternion& operator *= (float m) { multiply(m); return *this; }

private:
	float m_scalar;
	Vector3 m_vector;
};









inline const tQuaternion operator + (const tQuaternion& lhs, const tQuaternion& rhs)
{
	tQuaternion ret(lhs);
	ret += rhs;
	return ret;
}

inline const tQuaternion operator - (const tQuaternion& lhs, const tQuaternion& rhs)
{
	tQuaternion ret(lhs);
	ret -= rhs;
	return ret;
}

inline const tQuaternion operator * (const tQuaternion& lhs, const tQuaternion& rhs)
{
	tQuaternion ret(lhs);
	ret *= rhs;
	return ret;
}

inline const tQuaternion operator * (const tQuaternion& lhs, const float rhs)
{
	tQuaternion ret(lhs);
	ret *= rhs;
	return ret;
}

inline const tQuaternion operator * (const float lhs, const tQuaternion& rhs)
{
	tQuaternion ret(rhs);
	ret *= lhs;
	return ret;
}

inline const tQuaternion operator ~ (const tQuaternion& rhs) {
	tQuaternion ret(rhs);
	ret.conjugate();
	return ret;
}


#endif