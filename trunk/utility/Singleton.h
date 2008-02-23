#pragma once

template< class T >
class Singleton
{
public:

	T& instance() { return s_instance; }

	T& safeInstance() {
		if (!s_instance) s_instance = new T;
		return s_instance;
	}


protected:
	Singleton() { }
	~Singleton() { }

private:
	Singleton(const Singleton&);

	static T* s_instance;
};

template< class T >
T* Singleton< T >::s_instance = 0;