#include "ControllerManager.h"
#include "IController.h"

ControllerManager::ControllerManager() : m_activeController(NULL)
{
}

ControllerManager::~ControllerManager()
{
}


void ControllerManager::activateController(const std::string& name)
{
	activateController(findController(name));
}

void ControllerManager::activateController(IController* c)
{
	if (m_activeController != NULL) m_activeController->deactivate();
	m_activeController = c;
	m_activeController->activate();
}

void ControllerManager::update()
{
	if (m_activeController != NULL) m_activeController->update();
}

