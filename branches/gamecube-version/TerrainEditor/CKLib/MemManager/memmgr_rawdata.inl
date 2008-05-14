
// Member functions

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