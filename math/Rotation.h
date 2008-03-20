//*****************************************************************************
//**
//**   File:               Rotation.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "Quaternion.h"
class Vector3;
class Point3;
class Matrix44;

// Abstracts out the Quaternion from rotations
// Can use Matrix or 3 vectors for implementation instead if so desired

class Rotation
{
public:
	Rotation();
	Rotation(const Vector3& axis, float angle);
	~Rotation();

	Vector3 getAxis() const;
	float getAngle() const;
	Matrix44 getMatrix() const;
	Matrix44 getReverseMatrix() const;
	const Quaternion& getQuaternion() const { return m_q; }

	void clear();
	void set(const Vector3& axis, float angle);
	void set(float pitch, float yaw, float roll);

	void rotate(const Vector3& axis, float angle);
	void rotate(const Vector3& angle);
	void rotate(const Rotation& r);
	void rotateTowards(const Rotation& r, float time);

	void applyTo(Vector3& v) const;
	void applyTo(Point3& p) const; // assume rotation around origin

	void applyReverseTo(Vector3& v) const;
	void applyReverseTo(Point3& p) const;

private:
	Quaternion m_q;
};
