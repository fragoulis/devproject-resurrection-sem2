#pragma once
#include <list>

// Problem: don't know how to do templated singletons
// Sad face: have to put static in .cpp for every event type
// made a macro for it to make it easier

#define SIMPLE_EVENT_TYPE(name) class name { };
#define EVENT_TYPE(name, type) class name : public EventType<type> { };
#define REGISTER_EVENT(EventData) EventManager::EventManagerImpl<EventData> EventManager::EventManagerImpl<EventData>::s_instance;

template< typename EventData >
class EventListener
{
public:
	virtual void onEvent(EventData& eventdata) = 0;
};


// for making events, see macros above
template< typename T >
class EventType
{
public:
	EventType(const T& t) : m_t(t) { }
	inline T getData() { return m_t; }
private:
	T m_t;
};

//template< typename EventData >
//class ConstEventListener
//{
//public:
//	virtual void OnEvent(const EventData& eventdata) = 0;
//};




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
		typedef std::list<EventListener<EventData>*> EventListenerList;
		typedef typename EventListenerList::iterator EventListenerListIt;
		EventListenerList m_listeners;

		static EventManagerImpl<EventData> s_instance;

		EventManagerImpl() { }
		~EventManagerImpl() { }
	};


	// for adding static functions as event listeners
	// need code to delete em before I add this
	//template< typename EventData >
	//class StaticEventListener : EventListener<EventData>
	//{
	//public:
	//	typedef void (*)(EventData& eventdata) Function;
	//	StaticEventListener(Function func) : m_listenerFunction(func) { }
	//	virtual void onEvent(EventData& eventdata) {
	//		m_listenerFunction(eventdata);
	//	}
	//private:
	//	Function m_listenerFunction;
	//};


	EventManager() { }
	~EventManager() { }
};

