#pragma once

#include "IPostProcessFX.h"
#include "../GameLogic/GameEvents.h"
#include "../utility/EventManager.h"

/*
	screen noise grain effect
*/

class GrainPostProc : public IPostProcessFX, public EventListener<Player_EnergyDrained>
{
	Texture * m_noise;
	Texture * m_rand;

	bool   m_adjTrigger;
	float  m_noiseAdj;
	float  m_adjStartTime;

public :

	void onEvent(Player_EnergyDrained&);

	GrainPostProc();
	~GrainPostProc();
	Texture * process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const;
};