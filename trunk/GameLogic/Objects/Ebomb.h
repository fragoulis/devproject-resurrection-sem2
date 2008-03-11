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

	bool isToBeDeleted() const { return m_state == TO_BE_DELETED; }
	void setToBeDeleted() { m_state = TO_BE_DELETED; }


private:

	State m_state;
};
