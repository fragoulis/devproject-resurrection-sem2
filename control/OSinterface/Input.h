#ifndef _RES_INPUT_H_
#define _RES_INPUT_H_
#include "../../utility/singleton.h"
#include <dolphin/pad.h>

#define INPUT_FIRES_EVENTS 0

#ifdef INPUT_FIRES_EVENTS
	#include "../../utility/EventManager.h"
	EVENT_WITH_VALUE(Button_GoingDown, int);
#endif



/**
 * Provides an interface for OS specific code to send game pad data to the game
 * Provies an interface for the game to retrieve gamepad data
 *
 *
 * Extra numbers can be used for additional mouse buttons.
 */
class Input : public Singleton< Input >
{
public:
	inline bool isButtonGoingDown(int pad, int button) const;
	inline bool isButtonDown(int pad, int button) const;
	inline bool isButtonGoingUp(int pad, int button) const;
	inline bool isButtonUp(int pad, int button) const;

	inline int getControlX(int pad);
	inline int getControlY(int pad) const;
	inline int getCameraX(int pad) const;
	inline int getCameraY(int pad) const;
	inline int getTriggerL(int pad) const;
	inline int getTriggerR(int pad) const;

	inline bool isTriggerLGoingDown(int pad) const;
	inline bool isTriggerLDown(int pad) const;
	inline bool isTriggerRGoingDown(int pad) const;
	inline bool isTriggerRDown(int pad) const;

	inline int getPadCount() const;

	inline bool hasError(int pad) const;
	inline bool isValid(int pad) const;

	void update();


private:
	PADStatus m_currentPadz[PAD_MAX_CONTROLLERS];
	PADStatus m_previousPadz[PAD_MAX_CONTROLLERS];

	int m_padCount;


	friend Singleton< Input >;
	Input();
	~Input();
};



#include "Input.hpp"
#endif