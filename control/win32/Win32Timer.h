#pragma once
#include "../../utility/Exception.h"
#pragma warning( disable: 4290 )

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
