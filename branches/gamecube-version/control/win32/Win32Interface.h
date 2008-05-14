#pragma once
#include "../OSinterface/IOSInterface.h"


/**
 * Implements the IOSInterface interface
 * The OS creates an object of this type and sets it as the main OSInterface.
 */
class Win32Interface : public IOSInterface
{
public:
	Win32Interface();
	virtual ~Win32Interface();

	virtual void quit();
};
