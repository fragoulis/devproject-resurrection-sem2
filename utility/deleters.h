//*****************************************************************************
//**
//**   File:               deleters.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

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
