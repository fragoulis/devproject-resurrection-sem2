#include "Terrain.h"
#include "../../gfxutils/MemManager/MemMgr_RawData.h"


Terrain :: Terrain(const std::string& id)
:m_id(id),
m_terrainXZScale(0.0f),
m_terrainYMax(0),
m_terrainHeights(0),
m_terrainDim(0)
{
}

Terrain::~Terrain()
{
	// Delete / unload the terrain data
	if(m_terrainHeights)
	{
		MemMgrRaw::instance()->free(m_terrainHeights);
		m_terrainHeights = 0;
	}
}

float Terrain :: getHeight(float x, float z)
{
	const unsigned xgrid = unsigned(x/m_terrainXZScale)%m_terrainDim;
	const unsigned zgrid = unsigned(-z/m_terrainXZScale)/m_terrainDim;
	return m_terrainHeights[xgrid + m_terrainDim*zgrid];
}

void Terrain :: fillData(float * heights,const float xzscale,const float ymax,const unsigned dim)
{
	m_terrainXZScale = xzscale;
	m_terrainYMax = ymax;
	m_terrainDim = dim;
	m_terrainHeights = heights;
}