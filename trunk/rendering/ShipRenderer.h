//*****************************************************************************
//**
//**   File:               ShipRenderer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-11 23:43:03 +0000 (Tue, 11 Mar 2008) $
//**   Last Revision:      $Revision: 144 $
//**
//*****************************************************************************

#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "CoordinateModel.h"

#include <vector>

class Graphics;
struct EntitySettings_t;
class CoordinateFrame;
class ShipRenderer : public EventListener< Player_Spawned >,
					 public EventListener< Enemy_Spawned >,
					 public EventListener< Enemy_Despawned >,
					 public EventListener< Player_Despawned >,
					 public EventListener< Ebomb_Spawned >,
					 public EventListener< Ebomb_Despawned >,
					 public EventListener< Level_Unload >
					 
{
public:
	ShipRenderer();
	~ShipRenderer();

	void render(Graphics& g) const;

	void onEvent(Player_Spawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Despawned&);
	void onEvent(Player_Despawned&);
	void onEvent(Ebomb_Spawned&);
	void onEvent(Ebomb_Despawned&);
	void onEvent(Level_Unload&);

private:

	void _insertShip(const EntitySettings_t& settings, const CoordinateFrame * cframe);
	void _deleteShip(const CoordinateFrame * cframe);

	std::vector<CoordinateModel> m_ships;
};