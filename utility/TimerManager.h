#pragma once
#include "Singleton.h"
#include "Callbacks.h"
#include "Time.h"
#include <list>


/**
 * Allows scheduling of callback functions.
 *
 * Usage:
 * TimerManager::schedule(someObject, &SomeClass::DoStuffLater, afterXseconds);
 *
 */


class TimerManager
{
public:
	TimerManager();
	virtual ~TimerManager();

	int schedule(Callback*, float time, float period = -1.0f);
	void remove(int id);
	void setTime(int id, float time);

	void update(float dt);


	// Here to make your life a tiny bit easier
	template< typename ObjectType, typename Function >
	inline int schedule(ObjectType* obj, Function f, float time, float period = -1.0f);

	template< typename ObjectType, typename Function, typename BoundParam1 >
	inline int schedule(ObjectType* obj, Function f, BoundParam1 bp1, float time, float period = -1.0f);






private:



	class Timer
	{
	public:
		inline Timer(Callback* tc, float time, float period = -1.0f);
		inline ~Timer() { }

		inline float getTime() const { return m_time; }
		inline float getPeriod() const { return m_period; }
		inline int getID() const { return m_id; }
		inline bool isRepeating() const { return m_period > 0.0f; }

		inline void setTime(float time) { m_time = time; }
		inline void setPeriod(float f) { m_period = f; }
		inline void stopRepeating() { m_period = -1.0f; }

		void call() { m_callback->call(); }

		inline void create() { m_id = m_nextID++; }
		inline void destroy() { delete m_callback; }

		inline bool operator== (const int id) { return getID() == id; }

	private:
		float m_time;
		float m_period;
		Callback* m_callback;
		int m_id;

		static int m_nextID;
	};


	float m_time;
	typedef std::list<Timer> TimerList;
	typedef TimerList::iterator TimerListIt;
	TimerList m_timers;
};







template< typename ObjectType, typename Function >
inline int TimerManager :: schedule(ObjectType* obj, Function f, float time, float period)
{
	return schedule(Callback::create(obj, f), time, period);
}

template< typename ObjectType, typename Function, typename BoundParam1 >
inline int TimerManager :: schedule(ObjectType* obj, Function f, BoundParam1 bp1, float time, float period)
{
	return schedule(Callback::create(obj, f, bp1), time, period);
}

inline TimerManager::Timer::Timer(Callback* tc, float time, float period)
{
	m_callback = tc;
	m_period = period;
	m_time = time;
}
