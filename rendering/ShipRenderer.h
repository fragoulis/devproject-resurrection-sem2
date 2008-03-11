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
					 public EventListener< Enemy_Destroyed >,
					 public EventListener< Player_Destroyed >
					 
{
public:
	ShipRenderer();
	~ShipRenderer(){}

	void render(Graphics& g) const;

	void onEvent(Player_Spawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Destroyed&);
	void onEvent(Player_Destroyed&);

private:

	void _insertShip(const EntitySettings_t& settings, const CoordinateFrame * cframe);
	void _deleteShip(const CoordinateFrame * cframe);

	std::vector<CoordinateModel> m_ships;
};