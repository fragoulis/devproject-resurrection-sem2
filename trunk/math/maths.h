//*****************************************************************************
//**
//**   File:               maths.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

namespace Math
{
	const double PI = M_PI;
	const double PI_2 = M_PI_2;
	const double PI_4 = M_PI_4;
	const double E = M_E;
	const float almost_zero_f = 0.0001f;
	const double almost_zero_d = 0.0001;

	inline void srand(unsigned int seed) {
		std::srand(seed);
	}
	inline bool float_is_zero(float f) {
		return f > -almost_zero_f && f < almost_zero_f;
	}
	inline bool double_is_zero(double d) {
		return d > -almost_zero_d && d < almost_zero_d;
	}
	inline bool floats_are_equal(float a, float b) {
		return float_is_zero(a - b);
	}
	inline bool doubles_are_equal(double a, double b) {
		return double_is_zero(a - b);
	}
	inline bool float_is_bigger_or_equal(float a, float b) {
		return a > b || floats_are_equal(a, b);
	}
	inline bool double_is_bigger_or_equal(double a, double b) {
		return a > b || doubles_are_equal(a, b);
	}
	template< typename real >
	inline float clamp(real a, real minimum, real maximum) {
		return min(max(a, minimum), maximum);
	}

	inline float rand() {
		return float(std::rand()) / RAND_MAX;
	}
	template< typename T >
	inline T linterpolation_step(const T& t1, const T& t2, float time) {
		return (t2 - t1) / time;
	}
	template< typename T >
	inline T linterpolate(const T& t1, const T& t2, float amount, float max) {
		return t1 + linterpolation_step(t1, t2, max) * amount;
	}
	template< typename T >
	inline T linterpolate(const T& t1, const T& t2, float current, float begin, float end) {
		return linterpolate(t1, t2, current - begin, end - begin);
	}

	template< typename T >
	inline T moveTowards(const T& from, const T& to, const T& step) {
		if (to > from) return min(from + fabs(step * from), to);
		if (to < from) return max(from - fabs(step * from), to);
		return from;
	}
};