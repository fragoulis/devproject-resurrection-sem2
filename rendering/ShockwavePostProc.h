#pragma once

#include "IPostProcessFX.h"
#include "../GameLogic/GameEvents.h"
#include "../utility/EventManager.h"

class ShockwavePostProc : public IPostProcessFX, public EventListener<Player_Spawned>,
												 public EventListener<Player_Respawned>
{
	float m_startTime;
	bool  m_active;

	Texture * m_noiseTex;

public :

	ShockwavePostProc();
	~ShockwavePostProc();

	void onEvent(Player_Spawned&);
	void onEvent(Player_Respawned&);
	
	virtual Texture * process(Texture * input,Texture * output, FramebufferObject& fbo,const float additive_time) const;

};