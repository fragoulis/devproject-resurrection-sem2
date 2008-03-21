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


/**
 * Shows a loading screen, disables all user input.
 * Other controllers use this controller to handle the loading
 * The code that loads stuff is in a callback object called ILoader
 * For convenience a wrapper is provided that can wrap around
 * an object/method pair. That way you can have the LoadingController
 * call a method on an object to do the loading.
 *
 * Usage:
 * lc = new LoadingController();
 * lc->setLoader(this, &GameLogic::loadLevel);
 * ControllerManager::instance().activateController(lc);
 *
 * Where this is the GameLogic object.
 * ControllerManager
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
};


class LoadingController : public IController
{
public:
	LoadingController();
	virtual ~LoadingController();

	template< typename ObjectType, typename Function >
	void setLoader(ObjectType* obj, Function f) {
		_deleteLoader();
		m_loader = new ILoaderWrapper(obj, f);
		m_needToDeleteLoader = true;
	}
	void setLoader(ILoader* loader) {
		_deleteLoader();
		m_loader = loader;
	}

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
	ILoader* m_loader;
	bool m_needToDeleteLoader;
	float m_timeTillWeCanLoad;

	void _deleteLoader();
};
