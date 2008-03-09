//*****************************************************************************
//**
//**   File:               Laser.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../Movable.h"
#include "LaserTypes.h"
class ParserSection;

/**
 * All lasers are considered to be lines.
 * They have the following properties:
 * type, lifetime and length
 */
class Laser : public Movable
{
public:
	Laser();
	virtual ~Laser();

	float getLength() const { return m_length; }
	LaserType getLaserType() const { return m_type; }
	bool isDead() const { return m_age > m_lifetime; }

	void update(float dt) { m_age += dt; }

	void loadSettings(const ParserSection& ps);

private:
	LaserType m_type;
	float m_age;
	float m_lifetime;
	float m_length;
};
