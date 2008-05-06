#ifndef _matrix4_h_
#define _matrix4_h_

#include <Math/vec3.h>
#include <Math/vec4.h>

namespace CKLib {

template <class T>
class matrix44
{
private :
	//vec4<T> _rows[4];
	vec4<vec4<T> > _rows;
	
	static size_t  _size;

	const T _det3(const T* smat);
	void _extract_submat(T *mb, const int i, const int j );

public : 
	//ctors
	matrix44();		// Identity
	matrix44(const matrix44& mat);
	matrix44(const T mat[16]);
	matrix44(const T& a1,const T& a2,const T& a3,const T& a4,
			 const T& a5,const T& a6,const T& a7,const T& a8,
			 const T& a9,const T& a10,const T& a11,const T& a12,
			 const T& a13,const T& a14,const T& a15,const T& a16);
	matrix44(const vec4<T>& trans);		// set only a translation
	matrix44(const vec3<T>& trans);		// set only a translation

	// dtor
	~matrix44(){}

	// access 
	const T& operator ()(const unsigned& i, const unsigned& j) const;
	const vec3<T>& getRow3(const unsigned& i) const;
	const vec4<T>& getRow4(const unsigned& i) const;
	const T * const get() const;
	T& operator ()(const unsigned& i, const unsigned& j);
	vec3<T>& getRow3(const unsigned& i);
	vec4<T>& getRow4(const unsigned& i);

	// arithmetic
	const matrix44 operator * (const matrix44& mat) const;
	const matrix44& operator = (const matrix44& mat);
	const matrix44& operator *= (const matrix44& mat);

	const vec4<T> operator * (const vec4<T>& v) const;
	const vec3<T> operator * (const vec3<T>& v) const;
	
	// boolean
	const bool operator == (const matrix44& mat);
	const bool operator != (const matrix44& mat);
	const bool isZero() const;
	const bool isIdentity() const;


	//utilities
	const matrix44& loadZero();
	const matrix44& loadIdentity();
	const matrix44& transpose();
	const T determinant4();
	const matrix44& inverse();

	const vec3<T> rotate(const vec3<T>& v);

	/*
	// Standard matrices
	static matrix44 Identity();
	static matrix44 Zero();
	*/

	static const size_t size();
};

typedef matrix44<float> matrix44f;
typedef matrix44<double> matrix44d;

template <class T> size_t matrix44<T> :: _size = sizeof(matrix44<T>);

#include <Math/matrix4.inl>
#include <Math/matrix4.hpp>

}
#endif