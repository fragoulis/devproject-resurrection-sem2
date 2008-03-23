//*****************************************************************************
//**
//**   File:               CoordinateFrame.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 01:41:43 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 203 $
//**
//*****************************************************************************

#pragma once
#include "Point3.h"
#include "Rotation.h"
#include "Matrix44.h"


/**
 * Also known as Transform, Matrix44, PositionAndOrientationHelper
 * It helps with moving and rotating objects and
 * multiplying the OpenGL matrix stack
 *
 */
class CoordinateFrame
{
public:
	CoordinateFrame();
	~CoordinateFrame();

	const Point3& getOrigin() const { return m_origin; }
	const Rotation& getRotation() const { return m_rotation; }
	Vector3 getRotationAxis() const { return m_rotation.getAxis(); }
	float getRotationAngle() const { return m_rotation.getAngle(); }
	Matrix44 getMatrix() const;
	Matrix44 getReverseMatrix() const;

	void setOrigin(const Point3& p) { m_origin = p; _updateMatrix();}
	void setRotation(const Rotation& r) { m_rotation = r; _updateMatrix();}
	void setRotation(const Vector3& axis, float angle) { m_rotation.set(axis, angle); _updateMatrix();}

	void move(const Vector3& v);
	void rotate(const Vector3& axis, float angle);
	void rotate(const Vector3& angle);
	void rotate(const Rotation& r) { m_rotation.rotate(r); }
	void rotateTowards(const Rotation& r, float time) { m_rotation.rotateTowards(r, time); }

	void transform(Vector3& v) const;
	void transform(Point3& p) const;
	void reverseTransform(Vector3& v) const;
	void reverseTransform(Point3& p) const;

	float getX() const { return m_origin.getX(); }
	float getY() const { return m_origin.getY(); }
	float getZ() const { return m_origin.getZ(); }

	void setX(float x) { m_origin.setX(x); }
	void setY(float y) { m_origin.setY(y); }
	void setZ(float z) { m_origin.setZ(z); }

private:

	void _updateMatrix();

	Point3 m_origin;
	Rotation m_rotation;
	Matrix44 m_matrix;
};
