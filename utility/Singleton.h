//*****************************************************************************
//**
//**   File:               Singleton.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include <cassert>

template< class T >
class Singleton
{
public:

	//! Returns the object but also checks if the object exists
    static T& safeInstance()
    {
        if (s_instance == 0) create();
		return *s_instance;
	}

	//! Returns the object
	static T& instance() {
		assert(s_instance != 0);
		return *s_instance;
	}

	//! Creates the object
	static void create()
	{
		assert(s_instance == 0);
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