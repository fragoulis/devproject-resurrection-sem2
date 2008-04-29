#pragma once

#include <string>
#include <assert.h>
#include "../ConfParser/ConfParser.h"
#include "MemBinTree.h"

/*
	Raw Memory allocator implementing the buddy allocation scheme
	Used best for ?large? raw memory chunks
*/

class PareserSection;
class MemMgrRaw
{
	void * _data;
	unsigned _minMemoryBlock,	// smallest allocatable memblock 
			 _maxMemoryBlock,	// largest allocatable memblock
			 _stackSize;		// Memory (in elements) used by buddytree
	
	FixedMemTree * _buddytree;		// buddy system binary tree

	// static vars 
	static MemMgrRaw * _instance;	// entry point
	static bool _init;				// init flag

	// internal funcs
	void _parseConfiguration(const ParserSection * parsec);		// parses options

	// prohibited funcs
	MemMgrRaw(const ParserSection * parsec);			// ctor
	~MemMgrRaw();										// dtor
	MemMgrRaw(const MemMgrRaw& mgr):_buddytree(0){}		// copy ctor
	void operator = (const MemMgrRaw& mgr){}			// assign op


public :

	template <class T>
	T * allocate(const unsigned size);					// allocates size bytes, returns it in appr. form

	template <class T>
	void free(T * p);									// frees an allocated memory

	void clear();										// clears memory

	const MemTreeInfo& getStatus();						// gets the buddy tree status

	// Singleton Trio Stooges
	static void init(const ParserSection * parsec);		// init the raw manager
	static MemMgrRaw * instance();						// get the instance
	static void destroy();								// destroy the manager
};

template <class T>
inline T * MemMgrRaw :: allocate(const unsigned size)
{
	return reinterpret_cast<T *>(_buddytree->allocate(size*sizeof(T)));
}

template <class T>
inline void MemMgrRaw :: free(T * p)
{
	_buddytree->free(reinterpret_cast<char *>(p));
}

inline void MemMgrRaw :: clear()
{
	_buddytree->clear();
}

inline MemMgrRaw * MemMgrRaw :: instance()
{
	assert(_init);
	return _instance;
}

inline const MemTreeInfo& MemMgrRaw :: getStatus()
{
	return _buddytree->getStatus();
}

