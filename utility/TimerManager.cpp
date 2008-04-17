#include "TimerManager.h"

int TimerManager :: Timer :: m_nextID;

TimerManager :: TimerManager()
{
}

TimerManager :: ~TimerManager()
{
	// List contains values, not pointers
	// Timer destructor will delete the callback
}


int TimerManager :: add(Callback* cb, float time, float period)
{
	Timer timer(cb, time, period);
	timer.assignID();
	m_timers.push_back(timer);
	return timer.getID();
}
void TimerManager :: remove(int id)
{
	TimerListIt it = find(m_timers.begin(), m_timers.end(), id);
	if (it != m_timers.end()) m_timers.erase(it);
}
void TimerManager :: setTime(int id, float time)
{
	TimerListIt it = find(m_timers.begin(), m_timers.end(), id);
	if (it != m_timers.end()) it->setTime(m_time.getCurrentTime() + time);
}


void TimerManager :: update()
{
	float now = m_time.getCurrentTime();

	for (TimerListIt it = m_timers.begin(); it != m_timers.end(); )
	{
		Timer& timer = *it;
		if (timer.getTime() < now)
		{
			timer.call();
			if (timer.isRepeating())
			{
				timer.setTime(now + timer.getPeriod());
				++it;
			}
			else
			{
				it = m_timers.erase(it);
			}
		}
	}
}
