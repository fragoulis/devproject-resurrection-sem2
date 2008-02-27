#pragma once

#include "Singleton.h"
#include "../Math/Vector4.h"
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Math/Point3.h"
#include "../Math/Point2.h"

class RandomGenerator: public Singleton<RandomGenerator> {
public:

inline float getRandomFloat(float lowBound, float highBound) {
	if( lowBound >= highBound ) 
		return lowBound;
	float f = (rand() % 10000) * 0.0001f; 
	return (f * (highBound - lowBound)) + lowBound; 
}

inline void getRandomVector4(Vector4 &out, const Vector4 &min, const Vector4 &max) {
	out.set(getRandomFloat(min.getX(), max.getX()), getRandomFloat(min.getY(), max.getY()), getRandomFloat(min.getZ(), max.getZ()), 1.0f);
}

inline void getRandomVector3(Vector3 &out, const Vector3 &min, const Vector3 &max) {
	out.set(getRandomFloat(min.getX(), max.getX()), getRandomFloat(min.getY(), max.getY()), getRandomFloat(min.getZ(), max.getZ()));
}

inline void getRandomVector2(Vector2 &out, const Vector2 &min, const Vector2 &max) {
	out.set(getRandomFloat(min.getX(), max.getX()), getRandomFloat(min.getY(), max.getY()));
}

inline void getRandomPoint3(Point3 &out, const Point3 &min, const Point3 &max) {
	out.set(getRandomFloat(min.getX(), max.getX()), getRandomFloat(min.getY(), max.getY()), getRandomFloat(min.getZ(), max.getZ()));
}

inline void getRandomPoint2(Point2 &out, const Point2 &min, const Point2 &max) {
	out.set(getRandomFloat(min.getX(), max.getX()), getRandomFloat(min.getY(), max.getY()));
}


private:
	friend Singleton<RandomGenerator>; 

};

