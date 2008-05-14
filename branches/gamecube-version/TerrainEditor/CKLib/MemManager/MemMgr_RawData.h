#pragma once

#include <string>
#include <assert.h>
#include <ConfParser/ConfParser.h>
#include <DataStructs/MemBinTree.h>

namespace CKLib {

/*
	Raw memory allocator
	Used best for quick & dirty large mem chunks
	Uses the buddy allocation scheme
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

#include <MemManager/MemMgr_RawData.inl>

}