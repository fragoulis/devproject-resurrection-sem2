#pragma once
#include "../utility/ObjectReferenceMap.h"
#include "../utility/Singleton.h"
class IController;


/**
 * ControllerManager
 *
 * Loads, unloads controllers.
 * Exactly one controller is active at a time
 */
class ControllerManager : public Singleton< ControllerManager >
{
public:

	//! deactivates current controller, activates new controller. Loads if needed.
	void activateController(const std::string& name);
	
	//! loads a controller. Use for loading in advance
	void loadController(const std::string& name);

	//! unloads a controller. Use to free memory.
	void unloadController(const std::string& name);

	//! unloads all controllers. Use on program shutdown (also unloads active controller!)
	void unloadAllControllers();

	//! Sends update to active controller
	void update(float dt);


private:
	typedef ObjectReferenceMap<IController> ControllerMap;
	ControllerMap m_controllers;

	IController* m_activeController;



	// helper functions
	IController* findController(const std::string& name) { return m_controllers.findObjectReference(name); }
	IController* createController(const std::string& name) const;
	void addController(const std::string& name, IController* c) { m_controllers.addObjectReference(c, name); }
	void removeController(const std::string& name) { m_controllers.removeObjectReference(name); }
	void activateController(IController* c);

	friend Singleton< ControllerManager >;
	ControllerManager();
	~ControllerManager();
};
