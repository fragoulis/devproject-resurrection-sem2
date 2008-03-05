#pragma once
#include <list>

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


// Macros for quickly creating events

#define SIMPLE_EVENT(name) class name { };
#define EVENT_WITH_POINTER(name, type) \
	class name { \
	public: \
		name(type* t) : m_t(t) { } \
		type* getPointer() { return m_t; } \
	private: \
		type* m_t; \
	};

#define EVENT_WITH_2POINTERS(name, type1, type2) \
	class name { \
	public: \
		name(type1* t1, type2* t2) : m_t1(t1), m_t2(t2) { } \
		type1* getPointer1() { return m_t1; } \
		type2* getPointer2() { return m_t2; } \
	private: \
		type1* m_t1; \
		type2* m_t2; \
	};

#define EVENT_WITH_VALUE(name, type) \
	class name { \
	public: \
		name(const type& t) : m_t(t) { } \
		const type& getValue() { return m_t; } \
	private: \
		type m_t; \
	};

#define EVENT_WITH_CONST_REFERENCE(name, type) \
	class name { \
	public: \
		name(const type& t) : m_t(t) { } \
		const type& getReference() { return m_t; } \
	private: \
		const type& m_t; \
	};

#define EVENT_WITH_POINTER_AND_VALUE(name, ptype, vtype) \
	class name { \
	public: \
		name(ptype* pt, const vtype& vt) : m_pt(pt), m_vt(vt) { } \
		ptype* getPointer() { return m_pt; } \
		const vtype& getValue() { return m_vt; }  \
	private: \
		ptype* m_pt; \
		vtype m_vt; \
	};

#define EVENT_WITH_POINTER_AND_2VALUES(name, ptype, vtype1, vtype2) \
	class name { \
	public: \
		name(ptype* pt, const vtype1& vt1, const vtype2& vt2) : m_pt(pt), m_vt1(vt1), m_vt2(vt2) { } \
		ptype* getPointer() { return m_pt; } \
		const vtype1& getValue1() { return m_vt1; }  \
		const vtype2& getValue2() { return m_vt2; }  \
	private: \
		ptype* m_pt; \
		vtype1 m_vt1; \
		vtype2 m_vt2; \
	};




// for making events, see macros above
template< typename T >
class EventType
{
public:
	EventType(T t) : m_t(t) { }
	inline T getData() { return m_t; }
private:
	EventType() { }
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

template< class EventData >
EventManager::EventManagerImpl<EventData> EventManager::EventManagerImpl<EventData>::s_instance;