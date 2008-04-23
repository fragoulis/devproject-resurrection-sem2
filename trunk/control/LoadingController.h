//*****************************************************************************
//**
//**   File:               LoadingController.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#pragma once
#include "IController.h"
#include "../utility/Singleton.h"


/**
 * Shows a loading screen, disables all user input.
 * Other controllers use this controller to handle the loading
 * The code that loads stuff is in a callback object called ILoader
 * For convenience a wrapper is provided that can wrap around
 * an object/method pair. That way you can have the LoadingController
 * call a method on an object to do the loading.
 *
 * Usage:
 * LoadingController::safeInstance().load(someObject, &SomeClass::LoadStuff);
 *
 * This will show the loading screen,
 * and when ready, calls someObject->LoadStuff;
 */



class ILoader
{
public:
	ILoader() { }
	virtual ~ILoader() { }
	virtual void load() = 0;
};

template< typename ObjectType, typename Function >
class ILoaderWrapper : public ILoader
{
public:
	ILoaderWrapper(ObjectType* obj, Function f) : m_object(obj), m_function(f) { }
	virtual ~ILoaderWrapper() { }
	virtual void load() {
		(m_object->*m_function)();
	}
private:
	ObjectType* m_object;
	Function m_function;
};





class LoadingController :
	public IController,
	public Singleton< LoadingController>
{
public:

	template< typename ObjectType, typename Function >
	void setLoader(ObjectType* obj, Function f) {
		_deleteLoader();
		m_loader = new ILoaderWrapper<ObjectType, Function>(obj, f);
		m_needToDeleteLoader = true;
	}
	void setLoader(ILoader* loader) {
		_deleteLoader();
		m_loader = loader;
	}

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

	// helper for ease of use
	template< typename ObjectType, typename Function >
	void load(ObjectType* obj, Function f)
	{
		LoadingController& lc = LoadingController::instance();
		lc.setLoader(obj, f);
		ControllerManager::safeInstance().activateController(&lc);
	}

private:
	ILoader* m_loader;
	bool m_needToDeleteLoader;
	float m_timeTillWeCanLoad;

	void _deleteLoader();



	friend Singleton< LoadingController >;
	LoadingController();
	virtual ~LoadingController();
};
