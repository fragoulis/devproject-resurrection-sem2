template <class T>
MemMgrAny<T> * MemMgrAny<T> :: _instance(NULL);

template <class T>
bool MemMgrAny<T> :: _init(false);

// Ctor / Dtor

template <class T>
MemMgrAny<T> :: MemMgrAny(const std::string& classname,const ParserSection * parsec)
:_data(NULL),
_freestack(NULL),
_stacksize(0)
{
	_parseConfiguration(classname,parsec);
	_data = (T *)_mm_malloc(_stacksize * sizeof(T),16);
	assert(_stacksize);
	assert(_data);
	_freestack = new FixedStack<T *>(_stacksize);
	assert(_freestack);
	for(unsigned i=0;i<_stacksize;++i)
	{
		T * ptr = &_data[i];
		_freestack->push(ptr);
	}
	_freestack->clearStatus();
}

template <class T>
MemMgrAny<T> :: ~MemMgrAny()
{
	// Append log data to file/logger if in inspectmode??
	if(_data)
		::_mm_free(_data);
	if(_freestack)
		delete _freestack;
}

// Configuration parsing
template <class T>
void MemMgrAny<T> :: _parseConfiguration(const std::string& classname,const ParserSection * parsec)
{
	// Get Distribution
	_stacksize = FromString<unsigned>(parsec->getSection("MemDistribution")->getVal(classname));
}

// Static Functions

template <class T>
void MemMgrAny<T> :: init(const std::string& classname,const ParserSection * parsec)
{
	if(!_init)
	{
		_instance = new MemMgrAny<T>(classname,parsec);
		_init = true;
	}
}

template <class T>
inline MemMgrAny<T> * MemMgrAny<T> :: instance()
{
	assert(_init);
	return _instance;
}

template <class T>
void MemMgrAny<T> :: destroy()
{
	if(_init)
	{
		delete _instance;
		_init = false;
	}
}

// Member functions

template <class T>
inline T * MemMgrAny<T> :: allocate()
{
	return _freestack->pop();
}

template <class T>
inline void MemMgrAny<T> :: free(T * p)
{
	_freestack->push(p);
}

template <class T>
inline void MemMgrAny<T> :: clear()
{
	_freestack->clear();
}

template <class T>
const FixedStackStatus& MemMgrAny<T> :: getStatus()
{
	return _freestack->getStatus();
}