#ifndef _matrix3_h_
#define _matrix3_h_

#include <Math/vec3.h>
#include <Math/vec4.h>

namespace CKLib {

template <class T>
class matrix33
{
private :
	//vec4<T> _rows[4];
	vec3<vec3<T> > _rows;
	
	static size_t  _size;

public : 
	//ctors
	matrix33();		// Identity
	matrix33(const matrix33& mat);
	matrix33(const T mat[9]);
	matrix33(const T& a1,const T& a2,const T& a3,
			 const T& a4,const T& a5,const T& a6,
			 const T& a7,const T& a8,const T& a9);
	matrix33(const T& a);

	// dtor
	~matrix33(){}

	// access 
	const T& operator ()(const unsigned& i, const unsigned& j) const;
	const vec3<T>& getRow3(const unsigned& i) const;
	const T * const get() const;
	T& operator ()(const unsigned& i, const unsigned& j);
	vec3<T>& getRow3(const unsigned& i);

	// arithmetic
	const matrix33 operator * (const matrix33& mat) const;
	const matrix33& operator = (const matrix33& mat);
	const matrix33& operator *= (const matrix33& mat);

	const vec3<T> operator * (const vec3<T>& v) const;
	
	// boolean
	const bool operator == (const matrix33& mat);
	const bool operator != (const matrix33& mat);
	const bool isZero() const;
	const bool isIdentity() const;


	//utilities
	const matrix33& loadZero();
	const matrix33& loadIdentity();
	const matrix33& transpose();
	const T determinant3();
	const matrix33& inverse();

	/*
	// Standard matrices
	static matrix33 Identity();
	static matrix33 Zero();
	*/

	static const size_t size();
};

typedef matrix33<float> matrix33f;
typedef matrix33<double> matrix33d;

template <class T> size_t matrix33<T> :: _size = sizeof(matrix33<T>);

#include <Math/matrix3.inl>
#include <Math/matrix3.hpp>

}
#endif