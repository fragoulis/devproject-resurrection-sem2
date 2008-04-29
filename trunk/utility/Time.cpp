#include "Time.h"
#include <windows.h>
#pragma comment(lib, "Winmm.lib")

// Rewritten to use timeGetTime cuz QueryPerformanceCounter is broken on AMd multicore

//float frequency = 0;
//
//float GetSystemTimestamp() throw(Exception)
//{
//	LARGE_INTEGER t;
//	BOOL result = QueryPerformanceCounter(&t);
//	if (!result) throw Exception("Couldn't query performance counter time");
//	return float(t.QuadPart / frequency);
//}
//
//void GetSystemFrequency() throw(Exception)
//{
//	LARGE_INTEGER f;
//	BOOL result = QueryPerformanceFrequency(&f);
//	if (!result) throw Exception("Couldn't query performance counter frequency");
//	frequency = float(f.QuadPart);
//}

float GetTime()
{
	return float(timeGetTime()) / 1000.0f;
}


Time :: Time()
{
	timeBeginPeriod(1);
	//if (frequency == 0) GetSystemFrequency();
	reset();
}

Time :: ~Time()
{
	timeEndPeriod(1);
}

float Time::getCurrentTime()
{
	return GetTime() - m_starttime;
	//return GetSystemTimestamp() - m_starttime;
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
	//m_lasttime = m_starttime = GetSystemTimestamp();
}

