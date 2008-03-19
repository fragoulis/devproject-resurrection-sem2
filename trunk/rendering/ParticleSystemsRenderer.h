//*****************************************************************************
//**
//**   File:               ParticleSystemsRenderer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"
#include "../ParticleSystem/PS_Base.h"
#include "../utility/EventManager.h"
#include "../control/OSinterface/Input.h"
#include "../GameLogic/GameEvents.h"
#include <vector>

class ParticleSystemsRenderer : public IRenderer, public EventListener<Key_GoingDown>, 
												  public EventListener<Enemy_Despawned>,
												  public EventListener<Player_EnergyDrained>,
												  public EventListener<Player_Spawned>
{
public:
	//flag to check if a jet particle system is already created (there is only one needed)
	bool m_isJetCreated;

	ParticleSystemsRenderer();
	virtual ~ParticleSystemsRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	virtual void onEvent(Key_GoingDown&);
	virtual void onEvent(Enemy_Despawned&);
	virtual void onEvent(Player_EnergyDrained&);
	virtual void onEvent(Player_Spawned&);

private:
	// bunch of ParticleSystems here I guess
	std::vector<PS_Base *>	m_psList;
};
