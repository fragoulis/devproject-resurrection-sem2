#pragma once
#include "../math/CoordinateFrame.h"

class WorldObject
{
public:
	WorldObject();
	virtual ~WorldObject();

	Point3 getPosition() const { return m_coordinateFrame.getOrigin(); }
	void setPosition(const Point3& p) { m_coordinateFrame.setOrigin(p); }

	CoordinateFrame getCoordinateFrame() const { return m_coordinateFrame; }
	void setCoordinateFrame(const CoordinateFrame& cf) { m_coordinateFrame = cf; }

private:
	CoordinateFrame m_coordinateFrame;
};
