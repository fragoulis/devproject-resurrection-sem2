#ifndef _RES_SHIPRENDERER_H_
#define _RES_SHIPRENDERER_H_
#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "CoordinateModel.h"

#include <vector>


/**
 * Renders enemy ships.
 */


class Graphics;
struct EntitySettings_t;
class CoordinateFrame;
class ShipRenderer : public EventListener< Player_Spawned >,
					 public EventListener< Enemy_Spawned >,
					 public EventListener< Enemy_Destroyed >,
					 public EventListener< Enemy_Despawned >,
					 public EventListener< Player_Despawned >,
					 public EventListener< Ebomb_Spawned >,
					 public EventListener< Ebomb_Despawned >,
					 public EventListener< Level_Unload >,
					 public EventListener< Player_Destroyed >,
					 public EventListener< Player_Respawned >
					 
{
public:
	ShipRenderer();
	virtual ~ShipRenderer();

	void render(Graphics& g) const;

	void onEvent(Player_Spawned&);
	void onEvent(Player_Despawned&);
	void onEvent(Player_Destroyed&);
	void onEvent(Player_Respawned&);
	void onEvent(Enemy_Spawned&);
	void onEvent(Enemy_Despawned&);
	void onEvent(Enemy_Destroyed&);
	void onEvent(Ebomb_Spawned&);
	void onEvent(Ebomb_Despawned&);
	void onEvent(Level_Unload&);

private:

	void _insertShip(const EntitySettings_t& settings, const CoordinateFrame * cframe);
	void _deleteShip(const CoordinateFrame * cframe);

	std::vector<CoordinateModel> m_ships;
};

#endif
