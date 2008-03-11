//*****************************************************************************
//**
//**   File:               ShipRenderer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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
					 public EventListener< Player_Despawned >
					 
{
public:
	ShipRenderer();
	~ShipRenderer(){}

	void render(Graphics& g) const;

	void onEvent(Player_Spawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Despawned&);
	void onEvent(Player_Despawned&);

private:

	void _insertShip(const EntitySettings_t& settings, const CoordinateFrame * cframe);
	void _deleteShip(const CoordinateFrame * cframe);

	std::vector<CoordinateModel> m_ships;
};