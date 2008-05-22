

inline bool Input :: isButtonGoingDown(int pad, int button) const
{
	return PADButtonDown(m_previousPadz[pad].button, m_currentPadz[pad].button) & button;
}


inline bool Input :: isButtonDown(int pad, int button) const
{
	return m_currentPadz[pad].button & button;
}


inline bool Input :: isButtonGoingUp(int pad, int button) const
{
	return PADButtonUp(m_previousPadz[pad].button, m_currentPadz[pad].button) & button;
}


inline bool Input :: isButtonUp(int pad, int button) const
{
	//return ~(m_currentPadz[i].button) & button;
	return !isButtonDown(pad, button);
}


inline int Input :: getControlX(int pad)
{
	return m_currentPadz[pad].stickX;
}

inline int Input :: getControlY(int pad) const
{
	return m_currentPadz[pad].stickY;
}

inline int Input :: getCameraX(int pad) const
{
	return m_currentPadz[pad].substickX;
}

inline int Input :: getCameraY(int pad) const
{
	return m_currentPadz[pad].substickY;
}

inline int Input :: getTriggerR(int pad) const
{
	return m_currentPadz[pad].triggerLeft;
}

inline int Input :: getTriggerL(int pad) const
{
	return m_currentPadz[pad].triggerRight;
}
