//*****************************************************************************
//**
//**   File:               fixedstack.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once

#include "../Misc/defines.h"
//#include <Threading/CriticalSection.h>



struct FixedStackStatus
{
	unsigned used,
			 maxelems,
			 total_push,
			 total_pop,
			 peak;
	FixedStackStatus(const unsigned maxel):
	used(0),maxelems(maxel),total_push(0),total_pop(0),peak(0){}
};

template <class T>
class FixedStack
{
	T * _stack;
	unsigned _sp;
	//CriticalSection _cs;

	// Debug data
	FixedStackStatus _status;

public : 
	FixedStack(unsigned elemnum);
	~FixedStack();

	//  Normal funcs
	void push(T& elem);
	T& pop();
	void clear();
	void clearStatus();


	const FixedStackStatus& getStatus();

};

#include "FixedStack.hpp"

