
template <class T>
FixedStack<T> :: FixedStack(unsigned elemnum)
:_status(elemnum)
{
	assert(elemnum);
	_stack = new T[elemnum];
	_sp = 0;
}

template <class T>
FixedStack<T> :: ~FixedStack()
{
	delete []_stack;
}


// Debug Funcs

template <class T>
inline void FixedStack<T> :: push(T& elem)
{
	assert(_sp != _status.maxelems);

	_stack[_sp] = elem;
	++_sp;
	
#ifdef _DEBUG
	++_status.total_push;
#endif
}

template <class T>
inline T& FixedStack<T> :: pop()
{
	assert(_sp);
#ifdef _DEBUG
	++_status.total_pop;
	_status.peak = MAX(_status.peak,_status.maxelems - _sp - 1);
#endif

	--_sp;
	T& ptr =  _stack[_sp];

	return ptr;
}

template <class T>
void FixedStack<T> :: clear ()
{
	_sp = _status.maxelems;
#ifdef _DEBUG
	clearStatus();
#endif
}

template <class T>
const FixedStackStatus& FixedStack<T> :: getStatus()
{
	_status.used = _status.maxelems - _sp;
	return _status;
}

template <class T>
void FixedStack<T> :: clearStatus()
{
	_status.total_push = _status.total_pop = _status.peak = 0;
}