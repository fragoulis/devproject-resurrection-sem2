#ifndef _RES_EVENTMANAGER_H_
#define _RES_EVENTMANAGER_H_
#include <vector>

/**
 * EventManager.
 * To fire an event, you need to create an event first.
 * You can use the macros below for that, or define a new class yourself.
 * The class of the object that you pass to fireEvent specifies the type of event.
 * 
 * To listen to an event, you need to inherit from EventListener< EventType >
 * And register with the manager using registerListener< EventType >(me);
 * And implement the onEvent(EventType* data) method as specified by the EventListener interface.
 *
 *
 */

template< typename EventData >
class EventListener
{
public:
	virtual void onEvent(EventData& eventdata) = 0;
};

// for firing events on GC
#define FIRE_EVENT(name) { name evt; EventManager::instance().fireEvent(evt); }
#define FIRE_EVENT_VAL(name, val) { name evt(val); EventManager::instance().fireEvent(evt); }
#define FIRE_EVENT_2VALS(name, val1, val2) { name evt(val1, val2); EventManager::instance().fireEvent(evt); }

// Macros for quickly creating events

#define SIMPLE_EVENT(name) class name { };
#define EVENT_WITH_VALUE(name, type) \
	class name { \
	public: \
		name(type t) : m_t(t) { } \
		type getValue() { return m_t; } \
	private: \
		type m_t; \
	};

#define EVENT_WITH_2VALUES(name, type1, type2) \
	class name { \
	public: \
		name(type1 t1, type2 t2) : m_t1(t1), m_t2(t2) { } \
		type1 getValue1() { return m_t1; } \
		type2 getValue2() { return m_t2; } \
	private: \
		type1 m_t1; \
		type2 m_t2; \
	};

#define EVENT_WITH_3VALUES(name, type1, type2, type3) \
	class name { \
	public: \
		name(type1 t1, type2 t2, type3 t3) : m_t1(t1), m_t2(t2), m_t3(t3) { } \
		type1 getValue1() { return m_t1; } \
		type2 getValue2() { return m_t2; } \
		type3 getValue3() { return m_t3; } \
	private: \
		type1 m_t1; \
		type2 m_t2; \
		type3 m_t3; \
	};









class EventManager
{
public:
	static EventManager& instance() { return s_instance; }

	template< typename EventData >
	void registerEventListener(EventListener<EventData>* el) {
		EventManagerImpl<EventData>::instance().registerEventListener(el);
	}
	template< typename EventData >
	void unRegisterEventListener(EventListener<EventData>* el) {
		EventManagerImpl<EventData>::instance().unRegisterEventListener(el);
	}
	template< typename EventData >
	void fireEvent(EventData& eventdata) {
		EventManagerImpl<EventData>::instance().fireEvent(eventdata);
	}

private:
	static EventManager s_instance;

	template< typename EventData >
	class EventManagerImpl
	{
	public:
		static EventManagerImpl<EventData>& instance() { return s_instance; }

		void registerEventListener(EventListener<EventData>* el) {
			m_listeners.push_back(el);
		}
		void unRegisterEventListener(EventListener<EventData>* el) {
			m_listeners.remove(el);
		}
		void fireEvent(EventData& eventdata) {
			for (EventListenerListIt i = m_listeners.begin(); i != m_listeners.end(); i++) {
				(*i)->onEvent(eventdata);
			}
		}

	private:
		typedef std::vector<EventListener<EventData>*> EventListenerList;
		typedef typename EventListenerList::iterator EventListenerListIt;
		EventListenerList m_listeners;

		static EventManagerImpl<EventData> s_instance;

		EventManagerImpl() { }
		~EventManagerImpl() { }
	};



	EventManager() { }
	~EventManager() { }
};

template< class EventData >
EventManager::EventManagerImpl<EventData> EventManager::EventManagerImpl<EventData>::s_instance;


#endif