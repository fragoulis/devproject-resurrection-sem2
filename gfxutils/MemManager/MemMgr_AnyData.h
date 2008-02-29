#pragma once

#include <assert.h>
#include "../ConfParser/ConfParser.h"
#include "../Misc/utils.h"
#include "FixedStack.h"

/*
	MemMgrAny<T> :

	FreeStack using S.O.B. 

	Objects using this, should overload new operator with allocation from this manager,
	and usage of placement new operator

	Configuration is done on initialization
	pass the name of the class that will appear int the config mem distribution sector
	pass also the config file & the memory section of course

	e.g. MemMgrAny<Mesh>::init("Mesh",parser->getSection("MemManager:AnyData"))

	supports inspectMode, where it logs max allocations of the type & saves it somewhere
*/


template <class T>
class MemMgrAny
{
	// essential vars
	T * _data;							//data pool
	FixedStack<T *> * _freestack;		//stack in which the T pointers to the data will be stored
	unsigned _stacksize;				//pre-allocated stack size (in terms of T max elements)

	// static vars 
	static MemMgrAny * _instance;		// entry point
	static bool _init;					// singleton initialization flag

	// internal funcs
	void _parseConfiguration(const std::string& classname,const ParserSection * parsec);	// parses options

	// prohibited funcs
	MemMgrAny(const std::string& classname,const ParserSection * parsec);		// ctor
	~MemMgrAny();																// dtor
	MemMgrAny(const MemMgrAny& mgr){}											// copy ctor
	void operator = (const MemMgrAny& mgr){}									// assign op

public :

	T * allocate();		// allocates memory for T ( pop from free stack)
	void free(T * p);	// frees the T pointer ( push to free stack)
	void clear();		// clears all allocated T's

	const FixedStackStatus& getStatus();		// gets the stack status

	// Singleton Trio Stooges
	static void init(const std::string& classname,const ParserSection * parsec);	// init the <T> manager
	static MemMgrAny * instance();													// get the instance
	static void destroy();															// destroy the <T> manager
	
};

#include "../MemManager/MemMgr_AnyData.hpp"

