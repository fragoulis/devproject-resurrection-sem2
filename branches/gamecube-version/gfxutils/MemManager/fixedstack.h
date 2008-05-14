#pragma once

#include "../Misc/defines.h"
//#include <Threading/CriticalSection.h>

/*
	Status struct for the fixed-sized stack class
*/

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

/*
	Fixed stack class
*/

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

