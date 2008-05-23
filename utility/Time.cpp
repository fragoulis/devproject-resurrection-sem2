#include "Time.h"
#include <dolphin/os.h>


//static double div = double(u64(OS_TIMER_CLOCK >> shift));

static double GetTime()
{
	//return double(u64(OSGetTime() >> shift)) / div;
	return double(OSTicksToSeconds(double(OSGetTime())));
}


Time :: Time()
{
	reset();
}

Time :: ~Time()
{
}

double Time::getCurrentTime()
{
	return GetTime() - m_starttime;
}

double Time::getDeltatime()
{
	double time = getCurrentTime();
	double deltatime = time - m_lasttime;
	m_lasttime = time;
	return deltatime;
}

void Time::reset()
{
	m_starttime = GetTime();
	m_lasttime = 0.0f;
}

