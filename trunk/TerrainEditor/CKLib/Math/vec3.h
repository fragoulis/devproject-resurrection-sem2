#ifndef _vec3_h_
#define _vec3_h_

#include <xmmintrin.h>
#include <Misc/defines.h>
#include <iostream>

using namespace std;

namespace CKLib {

template <class T>
class vec3
{
private :
	
	T _v[4];

	static size_t _size;

	public : 
	
	const __m128& simd() const
	{return _simd;}
	__m128& simd()
	{return _simd;}

	// ctors
	vec3(const T& scalar = 0);
	vec3(const T& scalar1,const T& scalar2,const T& scalar3);
	vec3(const vec3<T>& v);

	// dtor
	~vec3(){}

	// access
	const T& operator ()(const unsigned i) const;
	T& operator()(const unsigned i);
	const T * const get() const;
	const T& x() const;
	T& x();
	const T& y() const;
	T& y();
	const T& z() const;
	T& z();
	const T& w() const;
	T& w();

	void assign(const T x,const T y,const T z);

	// arithmetic vec3 - vec3
	const vec3 operator +(const vec3& v) const;
	const vec3 operator -(const vec3& v) const;
	const vec3 operator /(const vec3& v) const;
	const vec3 operator *(const vec3& v) const;
	const vec3 operator ^(const vec3& s) const;
	const vec3& operator +=(const vec3& v);
	const vec3& operator -=(const vec3& v);
	const vec3& operator /=(const vec3& v);
	const vec3& operator *=(const vec3& v);
	const vec3& operator =(const vec3& v);
	const vec3& operator ^=(const vec3& s);

	// arithmetic vec3 - scalar
	const vec3 operator +(const T& s) const;
	const vec3 operator -(const T& s) const;
	const vec3 operator /(const T& s) const;
	const vec3 operator *(const T& s) const;
	const vec3 operator ^(const T& s) const;
	const vec3& operator +=(const T& s);
	const vec3& operator -=(const T& s);
	const vec3& operator /=(const T& s);
	const vec3& operator *=(const T& s);
	const vec3& operator ^=(const T& s);

	// boolean vec3 - vec3
	const bool operator ==(const vec3& v) const;
	const bool operator	!=(const vec3& v) const;
	const bool isZero() const;
	const bool isUnit() const;

	// utilities
	const T dot(const vec3& v) const;
	const vec3 cross(const vec3& v) const;	//computes cross & sets & returns this
	const T length() const;
	const T lengthSqr() const;
	const T distance(const vec3& v) const;
	const T distanceSqr(const vec3& v) const;
	const vec3 normalize();
	const vec3 normalize_check();		//checks for unit length first
	const vec3 normalize(const vec3& v); //normalizes v & sets & returns this
	const vec3 abs() const;				// returns the absolute values of the vector elements

	static unsigned size();
	static string delim();

};

typedef vec3<float> vec3f;
typedef vec3<double> vec3d;

template <class T> size_t vec3<T> :: _size = sizeof(vec3<T>);

#include <Math/vec3.inl>
#include <Math/vec3.hpp>
//#include <Math/vec3_float.h>

} // CKLib namespace

#endif
