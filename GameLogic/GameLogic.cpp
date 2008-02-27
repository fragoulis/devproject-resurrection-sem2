#include "GameLogic.h"
#include "Terrain.h"
#include "Playership.h"

GameLogic::GameLogic() : m_terrain(NULL), m_playership(NULL)
{
}

GameLogic::~GameLogic()
{
}


void GameLogic :: loadLevel(const std::string& id)
{
	// should have gotten unload before this
	assert(m_terrain == NULL);

	EventManager::instance().fireEvent(Level_Load(id));

	// TODO: load level data, get terrain id from that
	m_terrain = new Terrain(id);
	EventManager::instance().fireEvent(Terrain_Changed(m_terrain));

	m_playership = new Playership();
	EventManager::instance().fireEvent(Player_Spawned(m_playership));
}

void GameLogic :: unloadLevel()
{
	EventManager::instance().fireEvent(Level_Unload());
	despawnTerrain();
	despawnPlayer();
}



void GameLogic :: despawnTerrain()
{
	if (m_terrain != NULL) {
		delete m_terrain;
		m_terrain = NULL;
	}
}

void GameLogic :: despawnPlayer()
{
	if (m_playership != NULL) {
		delete m_playership;
		m_playership = NULL;
	}
}
