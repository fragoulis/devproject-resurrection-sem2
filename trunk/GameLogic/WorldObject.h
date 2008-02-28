#pragma once
#include "../math/CoordinateFrame.h"

class WorldObject
{
public:
	WorldObject();
	virtual ~WorldObject();

	const Point3& getPosition() const { return m_coordinateFrame.getOrigin(); }
	void setPosition(const Point3& p) { m_coordinateFrame.setOrigin(p); }

	const Rotation& getRotation() const { return m_coordinateFrame.getRotation(); }
	void setRotation(const Rotation& r) { m_coordinateFrame.setRotation(r); }

	void rotate(const Vector3& axis, float angle) { m_coordinateFrame.rotate(axis, angle); }

	const CoordinateFrame& getCoordinateFrame() const { return m_coordinateFrame; }
	void setCoordinateFrame(const CoordinateFrame& cf) { m_coordinateFrame = cf; }

private:
	CoordinateFrame m_coordinateFrame;
};
