#ifndef _RES_LOADING_CONTROLLER_H_
#define _RES_LOADING_CONTROLLER_H_
#include "ControllerManager.h"
#include "IController.h"
#include "../utility/Singleton.h"
#include "../utility/Callbacks.h"


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



class LoadingController :
	public IController,
	public Singleton< LoadingController>
{
public:

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);


	template< typename ObjectType, typename Function >
	void load(ObjectType* obj, Function f)
	{
		LoadingController& lc = LoadingController::instance();
		Callback* cb = Callback::create(obj, f);
		lc.m_loader = cb;
		ControllerManager::safeInstance().activateController(&lc);
	}

	// this doesn't work for some reason
	template< typename ObjectType, typename Function, typename BoundParam1 >
	void load(ObjectType* obj, Function f, BoundParam1 bp1)
	{
		LoadingController& lc = LoadingController::instance();
		Callback* cb = Callback::create(obj, f, bp1);
		lc.m_loader = cb;
		ControllerManager::safeInstance().activateController(&lc);
	}

private:
	Callback* m_loader;
	float m_timeTillWeCanLoad;

	void _deleteLoader();



	friend Singleton< LoadingController >;
	LoadingController();
	virtual ~LoadingController();
};



#endif