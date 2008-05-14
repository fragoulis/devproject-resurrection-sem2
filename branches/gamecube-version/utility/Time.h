#pragma once
#pragma warning( disable: 4290 )


/**
 * High performance timer used to get deltatime every frame.
 * Deltatime is passed to Application::update.
 * QueryPerformanceCounter doesn't work on AMD64 dual-core,
 * so switched back to timeGetTime.
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
