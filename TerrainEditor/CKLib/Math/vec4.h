#ifndef _vec4_h_
#define _vec4_h_

//#include <xmmintrin.h>
#include <Misc/defines.h>
#include <iostream>

using namespace std;

namespace CKLib {

template <class T>
class vec4
{
private :
	
	T _v[4];

	static size_t _size;

	public : 
	// ctors
	vec4(const T& scalar = 0);
	vec4(const T& scalar1,const T& scalar2,const T& scalar3,const T& scalar4);
	vec4(const vec4<T>& v);
	const T& x() const;
	T& x();
	const T& y() const;
	T& y();
	const T& z() const;
	T& z();
	const T& w() const;
	T& w();

	// dtor
	~vec4(){}

	// access
	const T& operator ()(const unsigned i) const;
	T& operator()(const unsigned i);
	const T * const get() const;

	// arithmetic vec4 - vec4
	const vec4 operator +(const vec4& v) const;
	const vec4 operator -(const vec4& v) const;
	const vec4 operator /(const vec4& v) const;
	const vec4 operator *(const vec4& v) const;
	const vec4 operator ^(const vec4& s) const;
	const vec4& operator +=(const vec4& v);
	const vec4& operator -=(const vec4& v);
	const vec4& operator /=(const vec4& v);
	const vec4& operator *=(const vec4& v);
	const vec4& operator =(const vec4& v);
	const vec4& operator ^=(const vec4& s);

	// arithmetic vec4 - scalar
	const vec4 operator +(const T& s) const;
	const vec4 operator -(const T& s) const;
	const vec4 operator /(const T& s) const;
	const vec4 operator *(const T& s) const;
	const vec4 operator ^(const T& s) const;
	const vec4& operator +=(const T& s);
	const vec4& operator -=(const T& s);
	const vec4& operator /=(const T& s);
	const vec4& operator *=(const T& s);
	const vec4& operator ^=(const T& s);

	// boolean vec4 - vec4
	const bool operator ==(const vec4& v) const;
	const bool operator	!=(const vec4& v) const;
	const bool isZero() const;
	const bool isUnit() const;

	// utilities
	const T dot(const vec4& v) const;
	const vec4 cross(const vec4& v);	//computes cross & sets & returns this
	const T length() const;
	const T lengthSqr() const;
	const T distance(const vec4& v) const;
	const T distanceSqr(const vec4& v) const;
	const vec4 normalize();
	const vec4 normalize_check();		//checks for unit length first
	const vec4 normalize(const vec4& v); //normalizes v & sets & returns this

	static unsigned size();
	static string delim();

};

typedef vec4<float> vec4f;
typedef vec4<double> vec4d;

template <class T> size_t vec4<T> :: _size = sizeof(vec4<T>);

#include <Math/vec4.inl>
#include <Math/vec4.hpp>

} // CKLib namespace

#endif
