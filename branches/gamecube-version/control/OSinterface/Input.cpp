#include "Input.h"


static u32 l_padChanMask[PAD_MAX_CONTROLLERS] =
{
    PAD_CHAN0_BIT, PAD_CHAN1_BIT, PAD_CHAN2_BIT, PAD_CHAN3_BIT
};


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

	u32 resetReq = 0; // for error handling

    m_padCount = 0;
    for ( int i = 0 ; i < PAD_MAX_CONTROLLERS ; i++ )
    {
        // Connection check
        if ( m_currentPadz[i].err == PAD_ERR_NONE ||
             m_currentPadz[i].err == PAD_ERR_TRANSFER )
        {
            m_padCount++;
        }
        else if ( m_currentPadz[i].err == PAD_ERR_NO_CONTROLLER )
        {
            resetReq |= l_padChanMask[i];
        }
    }

    // Try resetting pad channels which have been not valid
    if ( resetReq )
    {
        // Don't care return status
        // If FALSE, then reset again in next DEMOPadRead.
        PADReset( resetReq );
    }



	#ifdef INPUT_FIRES_EVENTS
		if (isButtonGoingDown(0, PAD_BUTTON_A)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_A);
		if (isButtonGoingDown(0, PAD_BUTTON_B)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_B);
		if (isButtonGoingDown(0, PAD_BUTTON_X)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_X);
		if (isButtonGoingDown(0, PAD_BUTTON_Y)) FIRE_EVENT_VAL(Button_GoingDown, PAD_BUTTON_Y);
		if (isButtonGoingDown(0, PAD_TRIGGER_Z)) FIRE_EVENT_VAL(Button_GoingDown, PAD_TRIGGER_Z);
	#endif
}
