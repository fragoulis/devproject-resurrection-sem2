#include "Win32Timer.h"
#include <windows.h>

float frequency = 0;

double GetSystemTimestamp() throw(Exception)
{
	LARGE_INTEGER t;
	BOOL result = QueryPerformanceCounter(&t);
	if (!result) throw Exception("Couldn't query performance counter time");
	return double(t.QuadPart / frequency);
}

void GetSystemFrequency() throw(Exception)
{
	LARGE_INTEGER f;
	BOOL result = QueryPerformanceFrequency(&f);
	if (!result) throw Exception("Couldn't query performance counter frequency");
	frequency = float(f.QuadPart);
}

Win32Timer::Win32Timer()
{
	if (frequency == 0) GetSystemFrequency();
	reset();
}

Win32Timer::~Win32Timer()
{
}

double Win32Timer::getCurrentTime()
{
	return GetSystemTimestamp() - m_starttime;
}

double Win32Timer::getDeltatime()
{
	double time = getCurrentTime();
	double deltatime = time - m_lasttime;
	m_lasttime = time;
	return deltatime;
}

void Win32Timer::reset()
{
	m_starttime = GetSystemTimestamp();
}

