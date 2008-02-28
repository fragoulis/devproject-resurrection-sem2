#include "MemMgr_RawData.h"
#include "../Misc/utils.h"


MemMgrRaw * MemMgrRaw :: _instance(NULL);
bool MemMgrRaw :: _init(false);

// Ctor / Dtor

MemMgrRaw :: MemMgrRaw(const ParserSection * parsec)
:_data(NULL),
_minMemoryBlock(16),
_maxMemoryBlock(0),
_stackSize(0),
_buddytree(0)
{
	_parseConfiguration(parsec);
	_data = _mm_malloc(_maxMemoryBlock,16);
	assert(_maxMemoryBlock);
	assert(_minMemoryBlock);
	assert(_data);
	_buddytree = new FixedMemTree(parsec,_maxMemoryBlock,_minMemoryBlock,(char *)_data);
	assert(_buddytree);
	_buddytree->clearStatus();
}

MemMgrRaw :: ~MemMgrRaw()
{
	// Append log data to file/logger if in inspectmode??
	if(_data)
		::_mm_free(_data);
	if(_buddytree)
		delete _buddytree;
}

// Configuration parsing

void MemMgrRaw :: _parseConfiguration(const ParserSection * parsec)
{
	// Get general vars
	_minMemoryBlock = 2<<(FromString<unsigned>(parsec->getVal("MinMemoryPOT"))-1);
	_maxMemoryBlock = 2<<(FromString<unsigned>(parsec->getVal("MaxMemoryPOT"))-1);
}

// Static Functions

void MemMgrRaw :: init(const ParserSection * parsec)
{
	if(!_init)
	{
		_instance = new MemMgrRaw(parsec);
		_init = true;
	}
}

void MemMgrRaw :: destroy()
{
	if(_init)
	{
		delete _instance;
		_init = false;
	}
}

