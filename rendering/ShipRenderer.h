#pragma once
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "CoordinateModel.h"

#include <vector>

class Graphics;
class ShipRenderer : public EventListener< Player_Spawned >,
					 public EventListener< Enemy_Spawned >,
					 public EventListener< Enemy_Destroyed >,
					 public EventListener< Player_Destroyed >
					 
{
public:
	ShipRenderer(){}
	~ShipRenderer(){}

	void render(Graphics& g) const;

	void onEvent(Player_Spawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Destroyed&);
	void onEvent(Player_Destroyed&);

private:

	std::vector<CoordinateModel> m_ships;
};