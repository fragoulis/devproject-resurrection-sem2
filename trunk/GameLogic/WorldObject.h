//*****************************************************************************
//**
//**   File:               WorldObject.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 01:41:43 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 203 $
//**
//*****************************************************************************

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

	const Rotation& getRotation() const { return m_coordinateFrame.getRotation(); }
	void rotate(const Vector3& axis, float angle) { return m_coordinateFrame.rotate(axis, angle); }
	void rotate(const Vector3& angle) { return m_coordinateFrame.rotate(angle); }
	void rotate(const Rotation& r) { m_coordinateFrame.rotate(r); }
	void rotateTowards(const Rotation& r, float time) { m_coordinateFrame.rotateTowards(r, time); }
	void setRotation(const Rotation& r) { m_coordinateFrame.setRotation(r); }

	const CoordinateFrame& getCoordinateFrame() const { return m_coordinateFrame; }
	void setCoordinateFrame(const CoordinateFrame& cf) { m_coordinateFrame = cf; }

	void loadSettings(const ParserSection&);

	int getType() const { return m_type; }
	void setType(int type) { m_type = type; }

	float getX() const { return m_coordinateFrame.getX(); }
	float getY() const { return m_coordinateFrame.getY(); }
	float getZ() const { return m_coordinateFrame.getZ(); }

	void setX(float x) { m_coordinateFrame.setX(x); }
	void setY(float y) { m_coordinateFrame.setY(y); }
	void setZ(float z) { m_coordinateFrame.setZ(z); }

private:
	CoordinateFrame m_coordinateFrame;
	int m_type;
};
