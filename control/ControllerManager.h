#ifndef _RES_CONTROLLER_MANAGER_H_
#define _RES_CONTROLLER_MANAGER_H_
#include "../utility/Singleton.h"
class IController;


/**
 * Application calls ControllerManager::update.
 * Keeps a pointer to the active controller,
 * and sends an update to it every frame.
 */
class ControllerManager : public Singleton< ControllerManager >
{
public:

	//! activates a new controller. deactivates and deleted the previous controller
	void activateController(IController* controller);

	//! Sends update to active controller
	void update(float dt);



private:
	IController* m_activeController;



	friend Singleton< ControllerManager >;
	ControllerManager();
	~ControllerManager();
};



#endif