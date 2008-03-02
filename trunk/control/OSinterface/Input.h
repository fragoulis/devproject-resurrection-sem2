#pragma once
#include "../../utility/singleton.h"

#define INPUT_FIRES_EVENTS 1

#ifdef INPUT_FIRES_EVENTS
	#include "../../utility/EventManager.h"
	EVENT_WITH_VALUE(Key_GoingDown, int);
	EVENT_WITH_VALUE(Mouse_GoingDown, int);
#endif




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

	void onKeyDown(int key) { m_keyDown[key] = true; }
	void onKeyUp(int key) { m_keyDown[key] = false; }

	void onMouseMove(int x, int y);
	void onMouseDown(int button) { onKeyDown(button); }
	void onMouseUp(int button) { onKeyUp(button); }

	int getMouseX() const { return m_mouseX; }
	int getMouseY() const { return m_mouseY; }

	bool isMouseButtonGoingDown(int button) const { isKeyGoingDown(button); }
	bool isMouseButtonDown(int button) const { isKeyDown(button); }
	bool isMouseButtonGoingUp(int button) const { isKeyGoingUp(button); }
	bool isMouseButtonUp(int button) const { isKeyUp(button); }

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

	int m_mouseX, m_mouseY;


	friend Singleton< Input >;
	Input();
	~Input();
};
