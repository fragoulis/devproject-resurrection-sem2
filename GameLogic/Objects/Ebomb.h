//*****************************************************************************
//**
//**   File:               Ebomb.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include "../Rigidbody.h"
#include "../EbombTypes.h"

class Ebomb : public Rigidbody
{
public:
	enum State {
		SPAWNING,
		ALIVE,
		DYING,
		TO_BE_DELETED,
		UNKNOWN,
	};

	Ebomb();
	virtual ~Ebomb();

	EbombType getType() const { return m_ebombType; }
	void setEbombType(EbombType ebombType) { m_ebombType = ebombType; }

	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }

	void loadSettings(const ParserSection&);

private:

	State m_state;
	EbombType m_ebombType;
	float m_radius;
};
