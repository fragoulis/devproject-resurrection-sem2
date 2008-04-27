//*****************************************************************************
//**
//**   File:               Laser.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************


#include "Laser.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../../math/maths.h"


Laser :: Laser(int type) : m_age(0.0f), m_state(STARTING)
{
	setType(type);
	m_power = 1.0f;
}

Laser :: ~Laser()
{
}

void Laser :: start( const Point3& pos, const Vector3& direction )
{
	m_age = 0.0f;
	m_backPoint = pos;
	setPosition(pos);
	m_state = STARTING;
	m_direction = direction;
}

void Laser :: die()
{
	m_state = ENDING;
	m_lifetime = m_age + getPosition().distance(m_backPoint) / m_speed;
}

void Laser :: update( float dt )
{
	m_age += dt;

	if (m_age > m_lifetime) m_state = TO_BE_DELETED;

	switch (m_state) {

		case STARTING :
		{
			setPosition(getPosition() + m_direction * m_speed * dt);
			if (getPosition().distance(m_backPoint) > m_maxLength) {
				m_state = ALIVE;
			}
			break;
		}

		case ALIVE :
		{
			Vector3 offset = m_direction * m_speed * dt;
			setPosition(getPosition() + offset);
			m_backPoint += offset;
			break;
		}

		case ENDING :
		{
			m_backPoint += m_direction * m_speed * dt;
			// if (front - back).dot(direction) < 0 --> delete ?
			// both should point in either: same direction, or opposite
			//if (Math::float_is_zero(m_backPoint.distance(getPosition()))) {
			//	m_state = TO_BE_DELETED;
			//}
			break;
		}
	}
}


void Laser :: loadSettings(const ParserSection& ps)
{
	m_lifetime = FromString<float>(ps.getVal("Lifetime"));
	m_maxLength = FromString<float>(ps.getVal("MaxLength"));
	m_width = FromString<float>(ps.getVal("Width"));
	m_speed = FromString<float>(ps.getVal("Speed"));
}