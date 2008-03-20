//*****************************************************************************
//**
//**   File:               Matrix44.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

/// OpenGL stores a 4x4 matrix as a float array. The elements are stored as in the diagram below.
/// [ a0, a4,  a8, a12 ]
/// [ a1, a5,  a9, a13 ]
/// [ a2, a6, a10, a14 ]
/// [ a3, a7, a11, a15 ]


class Matrix44
{
public:
	Matrix44();
	Matrix44(float diagonal); // all 0, diagonal = i
	Matrix44(const float components[16]);
	Matrix44(float f11, float f21, float f31, float f41, 
		     float f12, float f22, float f32, float f42, 
			 float f13, float f23, float f33, float f43, 
			 float f14, float f24, float f34, float f44);
	~Matrix44();

	float get(int index) const { return m_components[index]; }
	float get(int row, int column) const { return m_components[row + column * 4]; }
	void set(int index, float value) { m_components[index] = value; }
	void set(int row, int column, float value) { m_components[row + column * 4] = value; }

	void loadIdentity();
	
	void add(const Matrix44& m);
	void multiply(float f);
	void postMultiply(const Matrix44& m); /// (this)(m)
	//void preMultiply(const Matrix44& m); /// (m)(this)
	float determinant() const;
	void invert();
	//void transpose(); // not implemented

	const float* cfp() const { return m_components; }

	// not implemented
	//virtual std::istream& read(std::istream& is);
	//virtual std::ostream& write(std::ostream& os) const;


	Matrix44& operator += (const Matrix44& m) { add(m); return *this; }
	Matrix44& operator *= (float m) { multiply(m); return *this; }
	Matrix44& operator *= (const Matrix44& m) { postMultiply(m); return *this; }

private:
	/// Layout like OpenGL
	float m_components[16];
};










inline const Matrix44 operator + (const Matrix44& lhs, const Matrix44& rhs)
{
	Matrix44 ret(lhs);
	ret += rhs;
	return ret;
}
inline const Matrix44 operator * (const Matrix44& lhs, const float rhs)
{
	Matrix44 ret(lhs);
	ret *= rhs;
	return ret;
}
inline const Matrix44 operator * (const float lhs, const Matrix44& rhs)
{
	Matrix44 ret(rhs);
	ret *= lhs;
	return ret;
}
inline const Matrix44 operator * (const Matrix44& lhs, const Matrix44& rhs)
{
	Matrix44 ret(lhs);
	ret *= rhs;
	return ret;
}
