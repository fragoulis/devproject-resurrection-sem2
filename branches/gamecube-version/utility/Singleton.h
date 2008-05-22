#ifndef _RES_SINGLETON_H_
#define _RES_SINGLETON_H_


template< class T >
class Singleton
{
public:

	//! Returns the object but also checks if the object exists
    static T& safeInstance()
    {
        if( s_instance == 0 ) 
        {
            s_instance = new T;
        }
		return *s_instance;
	}

	//! Returns the object
	static T& instance()
	{
		//assert(s_instance != 0);
		return *s_instance;
	}

	static void create()
	{
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
    virtual ~Singleton() { }

    //! The object's unique instance
    static T* s_instance;

}; // end Singleton

template< class T >
T* Singleton<T>::s_instance = 0;


#endif