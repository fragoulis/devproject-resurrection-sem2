static const int TRIGGER_TRESHOLD = 10;

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

inline bool Input :: isTriggerLGoingDown(int pad) const
{
	return m_currentPadz[pad].triggerLeft > TRIGGER_TRESHOLD &&
		m_previousPadz[pad].triggerLeft < TRIGGER_TRESHOLD;
}

inline bool Input :: isTriggerLDown(int pad) const
{
	return m_currentPadz[pad].triggerLeft > TRIGGER_TRESHOLD;
}

inline bool Input :: isTriggerRGoingDown(int pad) const
{
	return m_currentPadz[pad].triggerRight > TRIGGER_TRESHOLD &&
		m_previousPadz[pad].triggerRight < TRIGGER_TRESHOLD;
}

inline bool Input :: isTriggerRDown(int pad) const
{
	return m_currentPadz[pad].triggerRight > TRIGGER_TRESHOLD;
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

inline int Input :: getTriggerL(int pad) const
{
	return m_currentPadz[pad].triggerLeft;
}

inline int Input :: getTriggerR(int pad) const
{
	return m_currentPadz[pad].triggerRight;
}


inline int Input :: getPadCount() const
{
	return m_padCount;
}

inline bool Input :: hasError(int pad) const
{
	return m_currentPadz[pad].err != PAD_ERR_NONE;
}

inline bool Input :: isValid(int pad) const
{
	return m_currentPadz[pad].err != PAD_ERR_NO_CONTROLLER;
}