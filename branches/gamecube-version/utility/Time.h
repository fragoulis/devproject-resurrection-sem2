#ifndef _RES_TIME_H_
#define _RES_TIME_H_

/**
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


#endif