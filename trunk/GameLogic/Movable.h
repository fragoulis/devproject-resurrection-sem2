//*****************************************************************************
//**
//**   File:               Movable.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#pragma once
#include "WorldObject.h"


/**
 * Extends WorldObject by giving it a velocity and angularVelocity.
 */
class Movable : public WorldObject
{
public:
	Movable();
	virtual ~Movable();

	const Vector3& getVelocity() const { return m_velocity; }
	void setVelocity(const Vector3& v) { m_velocity = v; }

	const Vector3& getAngularVelocity() const { return m_angularVelocity; }
	void setAngularVelocity(const Vector3& v) { m_angularVelocity = v; }


private:
	Vector3 m_velocity;
	Vector3 m_angularVelocity;
};
