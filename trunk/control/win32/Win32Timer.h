//*****************************************************************************
//**
//**   File:               Win32Timer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-11 14:29:29 +0000 (Tue, 11 Mar 2008) $
//**   Last Revision:      $Revision: 137 $
//**
//*****************************************************************************


#pragma once
#include "../../utility/Exception.h"
#pragma warning( disable: 4290 )


/**
 * High performance timer used to get deltatime every frame.
 * Deltatime is passed to Application::update.
 */
class Win32Timer
{
public:
	Win32Timer() throw(Exception);
	~Win32Timer();

	double getLasttime() { return m_lasttime; }
	double getCurrentTime() throw(Exception);
	double getDeltatime() throw(Exception);

	void reset();

private:
	double m_starttime;
	double m_lasttime;
};
