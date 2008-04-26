#pragma once

#include "../utility/EventManager.h"
#include "../GameLogic/GameEvents.h"
#include "../Math/Vector3.h"

/*
	Misc effects applied in objects / world
*/

class Texture;
class Playership;
class Graphics;
class Camera;
class MiscFXRenderer : public EventListener<Player_Spawned>,
					   public EventListener<Player_Respawned>

{
public : 
	MiscFXRenderer();
	~MiscFXRenderer();

	void onEvent(Player_Spawned&);
	void onEvent(Player_Respawned&);
	
	void render(Graphics& g) const;
	void update(const float dt);
	void setCamera(const Camera * cam) {m_camera = cam;}

private:

	static const int RAY_NUM = 14;

	const Playership * m_plship;
	const Camera * m_camera;
	Vector3	m_rays[RAY_NUM];		// rays heading outwards in a sphere
	Texture * m_rayTex;			// used in invulnerability effect
	float m_accumTime;

	bool m_godModeON;

};