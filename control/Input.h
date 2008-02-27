#pragma once
#include "../utility/singleton.h"


class Input : public Singleton< Input >
{
public:
	Input();
	~Input();

	bool isKeyGoingDown(int key) const { return m_keyState[key] == GOING_DOWN; }
	bool isKeyGoingUp(int key) const { return m_keyState[key] == GOING_UP; }
	bool isKeyDown(int key) const { return m_keyState[key] == DOWN; }
	bool isKeyUp(int key) const { return m_keyState[key] == UP; }

	void onKeyDown(int key) { m_keyDown[key] = true; }
	void onKeyUp(int key) { m_keyDown[key] = false; }

	void onMouseMove(int x, int y);

	int getMouseX() const { return m_mouseX; }
	int getMouseY() const { return m_mouseY; }

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
};
