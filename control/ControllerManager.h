#pragma once
#include "../utility/ObjectReferenceMap.h"
class IController;

class ControllerManager
{
public:
	static ControllerManager& instance() { return s_instance; }

	IController* getActiveController() const { return m_activeController; }
	IController* findController(const std::string& name) { return m_controllers.findObjectReference(name); }
	
	void addController(const std::string& name, IController* c) { m_controllers.addObjectReference(c, name); }
	void removeController(const std::string& name) { m_controllers.removeObjectReference(name); }

	void activateController(const std::string& name);
	void activateController(IController* c);

	void update();


private:
	typedef ObjectReferenceMap<IController> ControllerMap;
	ControllerMap m_controllers;

	IController* m_activeController;


	static ControllerManager s_instance;

	ControllerManager();
	~ControllerManager();
};
