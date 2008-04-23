//*****************************************************************************
//**
//**   File:               Ebomb.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 16:56:56 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 214 $
//**
//*****************************************************************************


#pragma once
#include "../Rigidbody.h"
#include "../EbombTypes.h"



/**
 * Represents a real e-bomb. That is, one that has been dropped by the player,
 * and has a visual representation on the screen.
 *
 */
class Ebomb : public Rigidbody
{
public:
	Ebomb();
	virtual ~Ebomb();

	float getRadius() const { return m_radius; }

	EbombType getEbombType() const { return m_ebombType; }
	void setEbombType(EbombType ebombType) { m_ebombType = ebombType; }

	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }

	void loadSettings(const ParserSection&);

private:
	enum State {
		SPAWNING, // not used yet
		ALIVE,
		DYING, // not used yet
		TO_BE_DELETED,
		UNKNOWN,
	};

	State m_state;
	EbombType m_ebombType;
	float m_radius;
};
