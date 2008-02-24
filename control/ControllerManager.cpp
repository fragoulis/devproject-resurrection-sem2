#include "ControllerManager.h"
#include "IController.h"
#include "LoadingController.h"
#include "GameController.h"
#include <cassert>

ControllerManager :: ControllerManager() : m_activeController(NULL)
{
	IController* c = createController("empty");
	addController("empty", c);
	m_activeController = c;
}

ControllerManager :: ~ControllerManager()
{
}


void ControllerManager :: activateController(const std::string& name)
{
	IController* c = findController(name);
	if (c == NULL) {
		c = createController(name);
		assert(c != NULL);
		addController(name, c);
	}
	activateController(c);
}

void ControllerManager :: loadController(const std::string& name)
{
	IController* c = findController(name);
	if (c == NULL) {
		c = createController(name);
		assert(c != NULL);
		addController(name, c);
	}
}

void ControllerManager :: unloadController(const std::string& name)
{
	IController* c = findController(name);
	assert(c != m_activeController);
	if (c != NULL) {
		removeController(name);
		delete c;
	}
}

void ControllerManager :: unloadAllControllers()
{
	struct ControllerDeleter
	{
		void operator () (const std::string&, IController* c) { delete c; }
	};
	m_controllers.visitObjects(ControllerDeleter());
}


void ControllerManager :: activateController(IController* c)
{
	m_activeController->deactivate();
	m_activeController = c;
	m_activeController->activate();
}

void ControllerManager::update()
{
	m_activeController->update();
}

class EmptyController : public IController
{
public:
	EmptyController() { }
	virtual ~EmptyController() { }
	virtual void activate() { }
	virtual void deactivate() { }
	virtual void update() { }
};

IController* ControllerManager::createController(const std::string &name) const
{
	if (name == "empty") return new EmptyController();
	if (name == "loading") return new LoadingController();
	if (name == "game") return new GameController();
	return NULL;
}