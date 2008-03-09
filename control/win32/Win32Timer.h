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
	unsigned long m_starttime;
	unsigned long m_lasttime;
};