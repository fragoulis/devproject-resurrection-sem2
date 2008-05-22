#include "Time.h"
#include <dolphin/os.h>

float GetTime()
{
	return OSTicksToSeconds(float(OSGetTime()));
}


Time :: Time()
{
	reset();
}

Time :: ~Time()
{
}

float Time::getCurrentTime()
{
	return GetTime() - m_starttime;
}

float Time::getDeltatime()
{
	float time = getCurrentTime();
	float deltatime = time - m_lasttime;
	m_lasttime = time;
	return deltatime;
}

void Time::reset()
{
	m_starttime = GetTime();
	m_lasttime = 0.0;
}

