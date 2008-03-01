#pragma once
#include "../OSinterface/IOSInterface.h"

class Win32Interface : public IOSInterface
{
public:
	Win32Interface();
	virtual ~Win32Interface();

	virtual void quit();
};
