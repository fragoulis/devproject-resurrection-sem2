/**
 * This adds 3 helper functions for deleting objects, lists of objects and vectors of objects
 * Has to be included after including <list> or <vector>
 */


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
        {
            T* item = *i;
            delete item;
            item = 0;
        }
		vec.clear();
	}
#endif

#ifdef _LIST_
	template< typename T >
	void deleteList(std::list<T*>& list)
	{
		for (std::list<T*>::iterator i = list.begin(); i != list.end(); ++i)
        {
            T* item = *i;
            delete item;
            item = 0;
        }
		list.clear();
	}
#endif
