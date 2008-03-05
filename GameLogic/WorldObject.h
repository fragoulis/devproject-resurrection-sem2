#pragma once
#include "../math/CoordinateFrame.h"
class ParserSection;

/**
 * Any game object with a position and orientation in the world is a WorldObject.
 */
class WorldObject
{
public:
	WorldObject();
	virtual ~WorldObject();

	const Point3& getPosition() const { return m_coordinateFrame.getOrigin(); }
	void setPosition(const Point3& p) { m_coordinateFrame.setOrigin(p); }

	const CoordinateFrame& getCoordinateFrame() const { return m_coordinateFrame; }
	void setCoordinateFrame(const CoordinateFrame& cf) { m_coordinateFrame = cf; }

	void loadSettings(const ParserSection&);

private:
	CoordinateFrame m_coordinateFrame;
};
