#include "ControllerManager.h"
#include "IController.h"

#include <iostream>
using namespace std;


class EmptyController : public IController
{
public:
	EmptyController() { }
	virtual ~EmptyController() { }
	virtual void activate() { }
	virtual void deactivate() { }
	virtual void update(float dt) { }
};

static EmptyController l_EmptyController;



ControllerManager :: ControllerManager() : m_activeController(0)
{
	m_activeController = &l_EmptyController;
}

ControllerManager :: ~ControllerManager()
{
	m_activeController->deactivate();
}

void ControllerManager::update(float dt)
{
	m_activeController->update(dt);
}


void ControllerManager :: activateController(IController* controller)
{
	m_activeController->deactivate();
	m_activeController = controller;
	m_activeController->activate();
}