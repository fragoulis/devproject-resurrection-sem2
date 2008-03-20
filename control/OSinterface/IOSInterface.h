//*****************************************************************************
//**
//**   File:               IOSInterface.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#pragma once
#include <cassert>

/**
 * Provides an interface for the game to talk to the OS.
 * The OS does not use this to talk to the game.
 * Instead, the OS uses Application to talk to the game.
 *
 * OS is responsible for instantiating a subclass of this,
 * calling setInstance, and deleting it on application shutdown.
 *
 * TODO: add get window size
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
