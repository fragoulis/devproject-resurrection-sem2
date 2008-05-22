#include "Input.h"

Input::Input()
{
	memset(m_currentPadz, 0, sizeof(PADStatus) * PAD_MAX_CONTROLLERS);
	//for (int i = 0; i < PAD_MAX_CONTROLLERS; i++)
	//{
	//	m_currentPadz[i].button = 0;
	//	m_previousPadz[i].button = 0;
	//}
}

Input::~Input()
{
}


void Input :: update()
{
	for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) m_previousPadz[i] = m_currentPadz[i];
	PADRead(m_currentPadz);
	PADClampCircle(m_currentPadz);

	#ifdef INPUT_FIRES_EVENTS
		if (isButtonGoingDown(0, PAD_BUTTON_A)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_A);
		if (isButtonGoingDown(0, PAD_BUTTON_B)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_B);
		if (isButtonGoingDown(0, PAD_BUTTON_X)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_X);
		if (isButtonGoingDown(0, PAD_BUTTON_Y)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_Y);
		if (isButtonGoingDown(0, PAD_TRIGGER_Z)) FIRE_EVENT_VAL(Button_GoingDown, PAD_TRIGGER_Z);
	#endif
}
