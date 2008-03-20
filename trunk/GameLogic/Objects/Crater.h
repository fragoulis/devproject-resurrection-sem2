//*****************************************************************************
//**
//**   File:               Crater.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#pragma once
#include "../WorldObject.h"
#include "../EbombTypes.h"
class ParserSection;

class Crater : public WorldObject
{
public:
	enum State {
		SPAWNING,
		ALIVE,
		DYING,
		TO_BE_DELETED,
		UNKNOWN,
	};


	Crater();
	virtual ~Crater();

	float getRadius() const { return m_radius; }
	EbombType getEbombType() const { return m_ebombType; }

	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }

	void loadSettings(const ParserSection&);

private:
	State m_state;
	float m_radius;
	EbombType m_ebombType;
};
