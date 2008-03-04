#pragma once
#include "Point3.h"
#include "Rotation.h"
#include "Matrix44.h"



class CoordinateFrame
{
public:
	CoordinateFrame();
	CoordinateFrame(const Point3& p, const Rotation& r);
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

	void transform(Vector3& v) const;
	void transform(Point3& p) const;
	void reverseTransform(Vector3& v) const;
	void reverseTransform(Point3& p) const;

private:

	void _updateMatrix();

	Point3 m_origin;
	Rotation m_rotation;
	Matrix44 m_matrix;
};
