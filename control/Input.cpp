#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input :: onMouseMove(int x, int y)
{
	m_mouseX = x;
	m_mouseY = y;
}

void Input :: update()
{
	// copy key state to previous key state
	for (int i = 0; i < NUM_KEYS; i++)
	{
		m_previousKeyState[i] = m_keyState[i];
		if (m_previousKeyState[i] == GOING_DOWN || m_previousKeyState[i] == DOWN)
		{
			if (m_keyDown[i]) {
				m_keyState[i] = DOWN;
			}
			else {
				m_keyState[i] = GOING_UP;
			}
		}

		if (m_previousKeyState[i] == GOING_UP || m_previousKeyState[i] == UP)
		{
			if (m_keyDown[i]) {
				m_keyState[i] = UP;
			}
			else {
				m_keyState[i] = GOING_DOWN;
#ifdef INPUT_FIRES_EVENTS
				if (i < 3) EventManager::instance().fireEvent(Mouse_GoingDown(i));
				else EventManager::instance().fireEvent(Key_GoingDown(i));
#endif
			}
		}
	}


	// TODO: poll for mouse position
}