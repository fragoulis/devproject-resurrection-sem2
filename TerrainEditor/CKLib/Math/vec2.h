#ifndef _vec2_h_
#define _vec2_h_

#include <Misc/defines.h>
#include <iostream>

using namespace std;

namespace CKLib {

template <class T>
class vec2
{
private :
	
	T _v[2];

	static size_t _size;

	public : 

	// ctors
	vec2(const T& scalar = 0);
	vec2(const T& scalar1,const T& scalar2);
	vec2(const vec2<T>& v);

	// dtor
	~vec2(){}

	// access
	const T& operator ()(const unsigned i) const;
	T& operator()(const unsigned i);
	const T * const get() const;
	const T& x() const;
	T& x();
	const T& y() const;
	T& y();

	// arithmetic vec2 - vec2
	const vec2 operator +(const vec2& v) const;
	const vec2 operator -(const vec2& v) const;
	const vec2 operator /(const vec2& v) const;
	const vec2 operator *(const vec2& v) const;
	const vec2 operator ^(const vec2& s) const;
	const vec2& operator +=(const vec2& v);
	const vec2& operator -=(const vec2& v);
	const vec2& operator /=(const vec2& v);
	const vec2& operator *=(const vec2& v);
	const vec2& operator =(const vec2& v);
	const vec2& operator ^=(const vec2& s);

	// arithmetic vec2 - scalar
	const vec2 operator +(const T& s) const;
	const vec2 operator -(const T& s) const;
	const vec2 operator /(const T& s) const;
	const vec2 operator *(const T& s) const;
	const vec2 operator ^(const T& s) const;
	const vec2& operator +=(const T& s);
	const vec2& operator -=(const T& s);
	const vec2& operator /=(const T& s);
	const vec2& operator *=(const T& s);
	const vec2& operator ^=(const T& s);

	// boolean vec2 - vec2
	const bool operator ==(const vec2& v) const;
	const bool operator	!=(const vec2& v) const;
	const bool isZero() const;
	const bool isUnit() const;

	// utilities
	const T dot(const vec2& v) const;
	const T length() const;
	const T lengthSqr() const;
	const T distance(const vec2& v) const;
	const T distanceSqr(const vec2& v) const;
	const vec2 normalize();
	const vec2 normalize_check();		//checks for unit length first
	const vec2 normalize(const vec2& v); //normalizes v & sets & returns this

	static unsigned size();
	static string delim();

};

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;

template <class T> size_t vec2<T> :: _size = sizeof(vec2<T>);

#include <Math/vec2.inl>
#include <Math/vec2.hpp>

} // CKLib namespace

#endif
