#pragma once
#include <cassert>

/**
 * Provides an interface to the OS
 *
 * OS is responsible for instantiating a subclass of this,
 * calling setInstance, and deleting it on application shutdown.
 *
 * TODO: add set resolution?
 */
class IOSInterface
{
public:

	//! This quits the application.
	virtual void quit() = 0;


	//! Called by the game to access the OS
	static IOSInterface* instance() {
		assert(s_instance != 0);
		return s_instance;
	}

	//! Called by the OS
	static void setInstance(IOSInterface* i) {
		assert(i != 0);
		assert(s_instance == 0);
		s_instance = i;
	}

private:
	static IOSInterface* s_instance;
};
