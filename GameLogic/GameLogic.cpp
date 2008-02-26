#include "GameLogic.h"
#include "Terrain.h"

GameLogic::GameLogic() : m_terrain(NULL)
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
}

void GameLogic :: unloadLevel()
{
	EventManager::instance().fireEvent(Level_Unload());
	despawnTerrain();
}



void GameLogic :: despawnTerrain()
{
	if (m_terrain != NULL) {
		delete m_terrain;
		m_terrain = NULL;
	}
}