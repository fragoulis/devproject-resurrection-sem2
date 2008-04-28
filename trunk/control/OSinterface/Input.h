//*****************************************************************************
//**
//**   File:               Input.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#pragma once
#include "../../utility/singleton.h"
#include "../../math/Point2.h"

#define INPUT_FIRES_EVENTS 1

#ifdef INPUT_FIRES_EVENTS
	#include "../../utility/EventManager.h"
	EVENT_WITH_VALUE(Key_GoingDown, int);
	EVENT_WITH_VALUE(Mouse_GoingDown, int);
#endif


// keys
#define KEY_ENTER   13
#define KEY_ESC     27
#define KEY_SPACE   32
#define KEY_LEFT    37
#define KEY_UP      38
#define KEY_RIGHT   39
#define KEY_DOWN    40

/**
 * Provides an interface for OS specific code to send keyboard/mouse data to the game
 * Provies an interface for the game to retrieve keyboard/mouse data
 *
 * Mouse buttons are numbered:
 * 1 - left
 * 2 - right
 *
 * Extra numbers can be used for additional mouse buttons.
 */
class Input : public Singleton< Input >
{
public:
	bool isKeyGoingDown(int key) const { return m_keyState[key] == GOING_DOWN; }
	bool isKeyGoingUp(int key) const { return m_keyState[key] == GOING_UP; }
	bool isKeyDown(int key) const { return m_keyState[key] == DOWN; }
	bool isKeyUp(int key) const { return m_keyState[key] == UP; }
	bool isKeyDownOrGoingDown(int key) const { return isKeyDown(key) || isKeyGoingDown(key); }
	bool isKeyUpOrGoingUp(int key) const { return isKeyUp(key) || isKeyGoingUp(key); }

	void onKeyDown(int key) { m_keyDown[key] = true; }
	void onKeyUp(int key) { m_keyDown[key] = false; }

	void onMouseMove(int x, int y) { m_mousePosition.set(float(x), float(y)); }
	void onMouseDown(int button) { onKeyDown(button); }
	void onMouseUp(int button) { onKeyUp(button); }

	Point2 getMousePosition() { return m_mousePosition; }

	bool isMouseButtonGoingDown(int button) const { return isKeyGoingDown(button); }
	bool isMouseButtonDown(int button) const { return isKeyDown(button); }
	bool isMouseButtonGoingUp(int button) const { return isKeyGoingUp(button); }
	bool isMouseButtonUp(int button) const { return isKeyUp(button); }

	void update();

private:
	static const int NUM_KEYS = 128;

	bool m_keyDown[NUM_KEYS];

	enum KeyState
	{
		GOING_DOWN,
		GOING_UP,
		UP,
		DOWN
	};
	KeyState m_keyState[NUM_KEYS];
	KeyState m_previousKeyState[NUM_KEYS];

	Point2 m_mousePosition;


	friend Singleton< Input >;
	Input();
	~Input();
};
