//*****************************************************************************
//**
//**   File:               Win32Interface.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


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
