//*****************************************************************************
//**
//**   File:               defines.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once

// ASSERTION 
#include <assert.h>

#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif
#endif

#include <cmath>
#include <cstdlib>

// MATH CONSTANTS


template <class T>
/*
	Class for wrapping up some math constants & fast random number acquisition
*/
class math_const
{
	static T _rand_list[65536];
	static unsigned short counter;
	static unsigned _randseed;
	static float _log2denom;
	public : 
	static void init_rand(const unsigned seed = 0);
	static const T DEG2RAD;
	static const T PI;
	static const T RANDMAX_TO_1;

	static const T rand_norm();
	static const unsigned rand_seed();
	static const T log2(const T num);
};
template <class T> const T math_const<T> :: DEG2RAD = T(0.017453292519943295769236907684886);
template <class T> const T math_const<T> :: PI = T(3.1415926535897932384626433832795);
template <class T> const T math_const<T> :: RANDMAX_TO_1 = T(1.0 / T(RAND_MAX));
template <class T> inline const T math_const<T> :: rand_norm() {return _rand_list[counter++];}
template <class T> float math_const<T> :: _log2denom = (1.0f) / (log(2.0f));
template <class T> const T math_const<T> :: log2(const T num) {return log(num)*_log2denom;}

// FLOATING POINT SPECIFIC

// MINIMUM FLOAT THRESHOLD 
template <class T>
inline const T THRESHOLD()
{
	return static_cast<const T>(1.0E-6f);
}

// Power Function ( + float specific)
template <class T,class U>
inline const T POW(const T& t1,const U& t2)
{
	return pow(t1,t2);
}

template <>
inline const float POW(const float& t1,const float& t2)
{
	return powf(t1,t2);
}

// Square Root Function ( + float specific)
template <class T>
inline const T SQRT(const T& t1)
{
	ASSERT(t1 > 0);
	return sqrt(t1);
}

template <>
inline const float SQRT(const float& t1)
{
	ASSERT(t1 >= 0.0f);
	return sqrtf(t1);
}

template <class T>
inline const T ABS(const T& t1)
{
	return abs(t1);
}

template <>
inline const float ABS(const float& t1)
{
	return fabs(t1);
}

// FLOAT / DOUBLE EQUALITY FUNCTION
template <class T>
inline bool float_equal(const T& v1,const T& v2)
{
	return ((v1 - v2) > THRESHOLD<T>())
		? false : ((v2 - v1) > THRESHOLD<T>() ) 
		? false : true ;
}

template <class T>
inline bool float_notequal(const T& v1,const T& v2)
{
	return ((v1 - v2) > THRESHOLD<T>())
		? true : ((v2 - v1) > THRESHOLD<T>()) 
		? true : false ;
}

template <class T,class U>
inline const U represent(const T& v)
{
	return *((const U*)(&v));
}

template <class T,class U>
inline const T MIN(const T& v1,const U& v2)
{
	return (v1 < T(v2)) ? v1 : T(v2);
}

template <class T,class U>
inline const T MAX(const T& v1,const U& v2)
{
	return (v1 > T(v2)) ? v1 : T(v2);
}

#include "../Misc/defines.hpp"

