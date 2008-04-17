//*****************************************************************************
//**
//**   File:               ParticleSystemsRenderer.h
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-19 11:53:22 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 191 $
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
												  public EventListener<Enemy_Spawned>,
												  public EventListener<Enemy_Despawned>,
												  public EventListener<Enemy_Destroyed>,
												  public EventListener<Player_EnergyDrained>,
												  public EventListener<Player_Spawned>,
												  public EventListener<Player_Respawned>,
												  public EventListener<Player_Destroyed>,
												  public EventListener<Ebomb_Despawned>,
												  public EventListener<Life_Restored>,
												  public EventListener<Terrain_Changed>,
												  public EventListener<Crater_Spawned>,
												  public EventListener<Level_Unload>
{
public:
	//flag to check if a jet particle system is already created (there is only one needed)
	bool m_isJetCreated;

	ParticleSystemsRenderer();
	virtual ~ParticleSystemsRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	virtual void onEvent(Key_GoingDown&);
	virtual void onEvent(Enemy_Spawned&);
	virtual void onEvent(Enemy_Despawned&);
	virtual void onEvent(Enemy_Destroyed&);
	virtual void onEvent(Player_EnergyDrained&);
	virtual void onEvent(Player_Spawned&);
	virtual void onEvent(Player_Respawned&);
	virtual void onEvent(Player_Destroyed&);
	virtual void onEvent(Ebomb_Despawned&);
	virtual void onEvent(Life_Restored&);
	virtual void onEvent(Terrain_Changed&);
	virtual void onEvent(Crater_Spawned&);
	virtual void onEvent(Level_Unload&);

private:
	// bunch of ParticleSystems here I guess
	typedef std::vector<PS_Base *> PSBaseVector;
	typedef PSBaseVector::iterator PSBaseVectorIt;
	PSBaseVector m_psList;

	void _removeEnemyViz(Enemyship* es);
};
