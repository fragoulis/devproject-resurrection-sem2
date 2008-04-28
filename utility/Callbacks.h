#pragma once

/**

These classes wrap callback functions.
The callback returns void.
You can bind 0, 1 or 2 parameters to it at creation, but not (yet) at call time.


**/

//template< typename ObjectType, typename Result, typename Param1 >
//struct mem_fun_binder11 : public unary_function< ObejctType, Result >
//{
//public:
//	explicit mem_fun_binder11(Result (ObjectType::*memfun)(Param1), Param1 p1) :
//		m_memfun(memfun), m_param1(p1) { }
//
//	Result operator()(ObjectType* obj) const
//	{
//		return (obj->*m_memfun)(m_param1);
//	}
//
//private:
//	Result (ObjectType::*m_memfun)(Param1);
//	Param1 m_param1
//};





/**

Callback is the interface for all void(void) type callbacks
The statics make it easier to create Callback objects.

**/



class Callback
{
public:
	virtual ~Callback() { }
	virtual void call() = 0;

	template< typename ObjectType, typename Function >
	inline static Callback* create(ObjectType* obj, Function f);

	template< typename ObjectType, typename Function, typename BoundParam1 >
	inline static Callback* create(ObjectType* obj, Function f, BoundParam1 bp1);

	template< typename ObjectType, typename Function, typename BoundParam1, typename BoundParam2 >
	inline static Callback* create(ObjectType* obj, Function f, BoundParam1 bp1, BoundParam2 bp2);
};







/*********************************************

Classes below implement the Callback interface of type void(void)
You can bind 0, 1 or 2 parameters when you create the callback.

*******************/



template< typename ObjectType, typename Function >
class Callback0 : public Callback
{
public:
	Callback0(ObjectType* obj, Function f) : m_object(obj), m_function(f) { }
	virtual ~Callback0() { }
	virtual void call() { (m_object->*m_function)(); }
private:
	ObjectType* m_object;
	Function m_function;
};

template< typename ObjectType, typename Function, typename BoundParam1 >
class Callback1 : public Callback
{
public:
	Callback1(ObjectType* obj, Function f, BoundParam1 bp1) : m_object(obj), m_function(f), m_param1(bp1) { }
	virtual ~Callback1() { }
	virtual void call() { (m_object->*m_function)(m_param1); }
private:
	ObjectType* m_object;
	Function m_function;
	BoundParam1 m_param1;
};

template< typename ObjectType, typename Function, typename BoundParam1, typename BoundParam2 >
class Callback2 : public Callback
{
public:
	Callback2(ObjectType* obj, Function f, BoundParam1 bp1, BoundParam2 bp2) : m_object(obj), m_function(f), m_param1(bp1), m_param2(bp2) { }
	virtual ~Callback2() { }
	virtual void call() { (m_object->*m_function)(m_param1, m_param2); }
private:
	ObjectType* m_object;
	Function m_function;
	BoundParam1 m_param1;
	BoundParam2 m_param2;
};









/**

Implementation of creation functions.
Simply does a new for the appropriate implementation of Callback

**/


template< typename ObjectType, typename Function >
inline static Callback* Callback :: create(ObjectType* obj, Function f)
{
	return new Callback0<ObjectType, Function>(obj, f);
}

template< typename ObjectType, typename Function, typename BoundParam1 >
inline static Callback* create(ObjectType* obj, Function f, BoundParam1 bp1)
{
	return new Callback1<ObjectType, Function, BoundParam1>(obj, f, bp1);
}

template< typename ObjectType, typename Function, typename BoundParam1, typename BoundParam2 >
inline static Callback* create(ObjectType* obj, Function f, BoundParam1 bp1, BoundParam2 bp2)
{
	return new Callback1<ObjectType, Function, BoundParam1, BoundParam2>(obj, f, bp1, bp2);
}
