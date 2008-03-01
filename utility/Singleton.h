#pragma once
#include <cassert>

template< class T >
class Singleton
{
public:

	//! Returns the object but also checks if the object exists
    static T& safeInstance()
    {
        if (s_instance == NULL) create();
		return *s_instance;
	}

	//! Returns the object
	static T& instance() {
		assert(s_instance != NULL);
		return *s_instance;
	}

	//! Creates the object
	static void create()
	{
		assert(s_instance == NULL);
		s_instance = new T;
	}

    //! Destroys the object
    static void destroy()
    {
        delete s_instance;
        s_instance = 0;
    }
    
protected:
    // Declare constructor, copy constructor,
    // assignment operator and destructor protected
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator= (const Singleton&);
    virtual ~Singleton() = 0 {}

    //! The object's unique instance
    static T* s_instance;

}; // end Singleton

template< class T >
T* Singleton<T>::s_instance = 0;