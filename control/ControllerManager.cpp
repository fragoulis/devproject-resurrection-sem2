//*****************************************************************************
//**
//**   File:               ControllerManager.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#include "ControllerManager.h"
#include "IController.h"
//#include "LoadingController.h"
//#include "GameController.h"
//#include "MenuController.h"
#include <cassert>


class EmptyController : public IController
{
public:
	EmptyController() { }
	virtual ~EmptyController() { }
	virtual void activate() { }
	virtual void deactivate() { }
	virtual void update(float dt) { }
};


EmptyController g_EmptyController;


ControllerManager :: ControllerManager() : m_activeController(NULL)
{
	//IController* c = createController("empty");
	//addController("empty", c);
	//m_activeController = c;
	m_activeController = &g_EmptyController;
}

ControllerManager :: ~ControllerManager()
{
	m_activeController->deactivate();
	//delete m_activeController;
}

void ControllerManager::update(float dt)
{
	m_activeController->update(dt);
}


void ControllerManager :: activateController(IController* controller)
{
	m_activeController->deactivate();
	//delete m_activeController;
	m_activeController = controller;
	m_activeController->activate();
}


//void ControllerManager :: activateController(const std::string& name)
//{
//	IController* c = findController(name);
//	if (c == NULL) {
//		c = createController(name);
//		assert(c != NULL);
//		addController(name, c);
//	}
//	activateController(c);
//}
//
//void ControllerManager :: loadController(const std::string& name)
//{
//	IController* c = findController(name);
//	if (c == NULL) {
//		c = createController(name);
//		assert(c != NULL);
//		addController(name, c);
//	}
//}
//
//void ControllerManager :: unloadController(const std::string& name)
//{
//	IController* c = findController(name);
//	assert(c != m_activeController);
//	if (c != NULL) {
//		removeController(name);
//		delete c;
//	}
//}
//
//void ControllerManager :: unloadAllControllers()
//{
//	struct ControllerDeleter
//	{
//		void operator () (const std::string&, IController* c) { delete c; }
//	};
//	m_controllers.visitObjects(ControllerDeleter());
//}
//
//
//void ControllerManager :: activateController(IController* c)
//{
//	m_activeController->deactivate();
//	m_activeController = c;
//	m_activeController->activate();
//}
//
//
//
//IController* ControllerManager::createController(const std::string &name) const
//{
//	if (name == "empty") return new EmptyController();
//	if (name == "loading") return new LoadingController();
//	if (name == "game") return new GameController();
//	if (name == "menu") return new MenuController();
//	return NULL;
//}