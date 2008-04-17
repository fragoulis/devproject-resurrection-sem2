//*****************************************************************************
//**
//**   File:               Timer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-11 14:29:29 +0000 (Tue, 11 Mar 2008) $
//**   Last Revision:      $Revision: 137 $
//**
//*****************************************************************************


#pragma once
#pragma warning( disable: 4290 )


/**
 * High performance timer used to get deltatime every frame.
 * Deltatime is passed to Application::update.
 */
class Time
{
public:
	Time();
	~Time();

	float getLasttime() { return m_lasttime; }
	float getCurrentTime();
	float getDeltatime();

	void reset();

private:
	float m_starttime;
	float m_lasttime;
};
