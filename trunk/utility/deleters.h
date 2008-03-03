

// Two helpers for deleting objects and lists
template< typename T >
void deleteObject(T*& t)
{
	if (t != NULL) {
		delete t;
		t = NULL;
	}
}

#ifdef _VECTOR_
	template< typename T >
	void deleteVector(std::vector<T*>& vec)
	{
		for (std::vector<T*>::iterator i = vec.begin(); i != vec.end(); ++i)
			delete *i;
		vec.clear();
	}
#endif

#ifdef _LIST_
	template< typename T >
	void deleteList(std::list<T*>& list)
	{
		for (std::list<T*>::iterator i = list.begin(); i != list.end(); ++i)
			delete *i;
		list.clear();
	}
#endif
