#pragma once

/// OpenGL stores a 3x3 matrix as a float array. The elements are stored as in the diagram below.
/// [ a0, a3, a6 ]
/// [ a1, a4, a7 ]
/// [ a2, a5, a8 ]


class Matrix33
{
public:
	Matrix33();
	Matrix33(const float components[9]);
	Matrix33(float f11, float f21, float f31,
		     float f12, float f22, float f32,
			 float f13, float f23, float f33);
	~Matrix33();

	float get(int index) const { return m_components[index]; }
	float get(int row, int column) const { return m_components[row + column * 3]; }
	void set(int index, float value) { m_components[index] = value; }
	void set(int row, int column, float value) { m_components[row + column * 3] = value; }
	void set(float f11, float f21, float f31,
		     float f12, float f22, float f32,
			 float f13, float f23, float f33);

	void loadIdentity();
	
	void add(const Matrix33& m);
	void multiply(float f);
	void postMultiply(const Matrix33& m); /// (this)(m)
	//void preMultiply(const Matrix33& m); /// (m)(this)
	float determinant() const;
	void invert();
	//void transpose(); // not implemented

	const float* cfp() const { return m_components; }

	// not implemented
	//virtual std::istream& read(std::istream& is);
	//virtual std::ostream& write(std::ostream& os) const;


	Matrix33& operator += (const Matrix33& m) { add(m); return *this; }
	Matrix33& operator *= (float m) { multiply(m); return *this; }
	Matrix33& operator *= (const Matrix33& m) { postMultiply(m); return *this; }

private:
	/// Layout like OpenGL
	float m_components[9];
};










inline const Matrix33 operator + (const Matrix33& lhs, const Matrix33& rhs)
{
	Matrix33 ret(lhs);
	ret += rhs;
	return ret;
}
inline const Matrix33 operator * (const Matrix33& lhs, const float rhs)
{
	Matrix33 ret(lhs);
	ret *= rhs;
	return ret;
}
inline const Matrix33 operator * (const float lhs, const Matrix33& rhs)
{
	Matrix33 ret(rhs);
	ret *= lhs;
	return ret;
}
inline const Matrix33 operator * (const Matrix33& lhs, const Matrix33& rhs)
{
	Matrix33 ret(lhs);
	ret *= rhs;
	return ret;
}
