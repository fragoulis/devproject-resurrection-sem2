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

	double getLasttime() { return m_lasttime; }
	double getCurrentTime();
	double getDeltatime();

	void reset();

private:
	double m_starttime;
	double m_lasttime;
};


#endif