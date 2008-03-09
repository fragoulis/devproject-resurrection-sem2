//*****************************************************************************
//**
//**   File:               Win32Timer.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#include "Win32Timer.h"
#include <windows.h>
#pragma comment(lib, "Winmm.lib")

// Rewritten to use timeGetTime cuz QueryPerformanceCounter is broken on AMd multicore

//float frequency = 0;
//
//double GetSystemTimestamp() throw(Exception)
//{
//	LARGE_INTEGER t;
//	BOOL result = QueryPerformanceCounter(&t);
//	if (!result) throw Exception("Couldn't query performance counter time");
//	return double(t.QuadPart / frequency);
//}
//
//void GetSystemFrequency() throw(Exception)
//{
//	LARGE_INTEGER f;
//	BOOL result = QueryPerformanceFrequency(&f);
//	if (!result) throw Exception("Couldn't query performance counter frequency");
//	frequency = float(f.QuadPart);
//}




Win32Timer::Win32Timer()
{
	timeBeginPeriod(1);
	//if (frequency == 0) GetSystemFrequency();
	reset();
}

Win32Timer::~Win32Timer()
{
	timeEndPeriod(1);
}

double Win32Timer::getCurrentTime()
{
	return (timeGetTime() - m_starttime) / 1000;
	//return GetSystemTimestamp() - m_starttime;
}

double Win32Timer::getDeltatime()
{
	//double time = getCurrentTime();
	//double deltatime = time - m_lasttime;
	//m_lasttime = timeGetTime();
	DWORD time = timeGetTime() - m_starttime;
	DWORD deltatime = time - m_lasttime;
	m_lasttime = time;
	return double(deltatime) / double(1000);
}

void Win32Timer::reset()
{
	m_lasttime = m_starttime = timeGetTime();
	//m_lasttime = m_starttime = GetSystemTimestamp();
}

