#pragma once

#include "Singleton.h"
#include "../Math/Vector4.h"
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Math/Point3.h"
#include "../Math/Point2.h"

namespace RandomGenerator {

inline float GET_RANDOM_FLOAT(float lowBound, float highBound) {
	if( lowBound >= highBound ) 
		return lowBound;
	float f = (rand() % 10000) * 0.0001f; 
	return (f * (highBound - lowBound)) + lowBound; 
}

inline void GET_RANDOM_VECTOR4(Vector4 &out, const Vector4 &min, const Vector4 &max) {
	out.set(GET_RANDOM_FLOAT(min.getX(), max.getX()), GET_RANDOM_FLOAT(min.getY(), max.getY()), GET_RANDOM_FLOAT(min.getZ(), max.getZ()), 1.0f);
}

inline void GET_RANDOM_VECTOR3(Vector3 &out, const Vector3 &min, const Vector3 &max) {
	out.set(GET_RANDOM_FLOAT(min.getX(), max.getX()), GET_RANDOM_FLOAT(min.getY(), max.getY()), GET_RANDOM_FLOAT(min.getZ(), max.getZ()));
}

inline void GET_RANDOM_VECTOR2(Vector2 &out, const Vector2 &min, const Vector2 &max) {
	out.set(GET_RANDOM_FLOAT(min.getX(), max.getX()), GET_RANDOM_FLOAT(min.getY(), max.getY()));
}

inline void GET_RANDOM_POINT3(Point3 &out, const Point3 &min, const Point3 &max) {
	out.set(GET_RANDOM_FLOAT(min.getX(), max.getX()), GET_RANDOM_FLOAT(min.getY(), max.getY()), GET_RANDOM_FLOAT(min.getZ(), max.getZ()));
}

inline void GET_RANDOM_POINT2(Point2 &out, const Point2 &min, const Point2 &max) {
	out.set(GET_RANDOM_FLOAT(min.getX(), max.getX()), GET_RANDOM_FLOAT(min.getY(), max.getY()));
}

};

