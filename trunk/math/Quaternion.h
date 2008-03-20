//*****************************************************************************
//**
//**   File:               Quaternion.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 01:41:43 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 203 $
//**
//*****************************************************************************

#pragma once
#include "Vector3.h"

class Quaternion
{
public:
	explicit Quaternion() { }
	Quaternion(float s, const Vector3& v);
	Quaternion(float s, float v1, float v2, float v3);
	~Quaternion() { }

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

	void add(const Quaternion& q);
	void subtract(const Quaternion& q);
	void multiply(const Quaternion& q); // Grassmann
	void multiply(float m);
	void conjugate();
	void normalize();
	void slerp(const Quaternion& target, float time);

	float squareLength() const;
	float length() const;


	Quaternion& operator += (const Quaternion& rhs) { add(rhs); return *this; }
	Quaternion& operator -= (const Quaternion& rhs) { subtract(rhs); return *this; }
	Quaternion& operator *= (const Quaternion& rhs) { multiply(rhs); return *this; }
	Quaternion& operator *= (float m) { multiply(m); return *this; }

private:
	float m_scalar;
	Vector3 m_vector;
};









inline const Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion ret(lhs);
	ret += rhs;
	return ret;
}

inline const Quaternion operator - (const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion ret(lhs);
	ret -= rhs;
	return ret;
}

inline const Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion ret(lhs);
	ret *= rhs;
	return ret;
}

inline const Quaternion operator * (const Quaternion& lhs, const float rhs)
{
	Quaternion ret(lhs);
	ret *= rhs;
	return ret;
}

inline const Quaternion operator * (const float lhs, const Quaternion& rhs)
{
	Quaternion ret(rhs);
	ret *= lhs;
	return ret;
}

inline const Quaternion operator ~ (const Quaternion& rhs) {
	Quaternion ret(rhs);
	ret.conjugate();
	return ret;
}
