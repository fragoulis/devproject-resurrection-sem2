//*****************************************************************************
//**
//**   File:               Timer.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-13 00:18:54 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 156 $
//**
//*****************************************************************************


#include "Timer.h"
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

double GetTime()
{
	return double(timeGetTime()) / 1000.0;
}


Timer :: Timer()
{
	timeBeginPeriod(1);
	//if (frequency == 0) GetSystemFrequency();
	reset();
}

Timer::~Timer()
{
	timeEndPeriod(1);
}

double Timer::getCurrentTime()
{
	return GetTime() - m_starttime;
	//return GetSystemTimestamp() - m_starttime;
}

double Timer::getDeltatime()
{
	double time = getCurrentTime();
	double deltatime = time - m_lasttime;
	m_lasttime = time;
	return deltatime;
}

void Timer::reset()
{
	m_starttime = GetTime();
	m_lasttime = 0.0;
	//m_lasttime = m_starttime = GetSystemTimestamp();
}

