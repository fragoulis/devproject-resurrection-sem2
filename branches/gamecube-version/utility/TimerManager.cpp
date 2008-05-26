#include "TimerManager.h"
#include <algorithm>
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"
#include <iostream>
using namespace std;

int TimerManager :: Timer :: m_nextID = 1;

TimerManager :: TimerManager() : m_time(0.0f)
{
	
}

TimerManager :: ~TimerManager()
{
	// List contains values, not pointers
	// Timer destructor will delete the callback
}


int TimerManager :: schedule(Callback* cb, float time, float period)
{
	Timer timer(cb, m_time + time, period);
	timer.create();
	m_timers.push_back(timer);
	//cout << "Scheduled a timer at " << int(m_time * 10.0f) << " for " << int(timer.getTime() * 10.0f) << endl;
	return timer.getID();
}
void TimerManager :: remove(int id)
{
	TimerListIt it = find(m_timers.begin(), m_timers.end(), id);
	if (it != m_timers.end()) m_timers.erase(it);
}

void TimerManager :: removeAll()
{
	m_timers.clear();
}

void TimerManager :: setTime(int id, float time)
{
	TimerListIt it = find(m_timers.begin(), m_timers.end(), id);
	if (it != m_timers.end()) it->setTime(m_time + time);
}


void TimerManager :: update(float dt)
{
	m_time += dt;

	for (TimerListIt it = m_timers.begin(); it != m_timers.end(); )
	{
		Timer& timer = *it;
		if (timer.getTime() < m_time)
		{
			timer.call();
			if (timer.isRepeating())
			{
				timer.setTime(m_time + timer.getPeriod());
				++it;
			}
			else
			{
				timer.destroy();
				it = m_timers.erase(it);
			}
		}
		else
		{
			//cout << "At " << i0nt(m_time * 10.0f) << " timer at " << int(timer.getTime() * 10.0f) << " is not ready yet" << endl;
			++it;
		}
	}
}
