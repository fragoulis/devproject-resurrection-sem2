#include "Terrain.h"
#include "../../utility/assert.h"
//#include "../../gfxutils/MemManager/MemMgr_RawData.h"


Terrain :: Terrain() :
	m_terrainXZScale(0.0f),
	m_terrainYMax(0),
	m_terrainHeights(0),
	m_terrainDim(0)
{
}

Terrain::~Terrain()
{
}

float Terrain :: getHeight(float x, float z)
{
	return 0.0f;
	assert(m_terrainHeights != 0);
	const unsigned xgrid = std::min(m_terrainDim-1,unsigned(m_terrainDim*(x + m_terrainXZScale*0.5f) / m_terrainXZScale));
	const unsigned zgrid = std::min(m_terrainDim-1,unsigned(m_terrainDim*(-z + m_terrainXZScale*0.5f) / m_terrainXZScale));
	return m_terrainHeights[xgrid + m_terrainDim*zgrid];
}

void Terrain :: fillData(float * heights,const float xzscale,const float ymax,const unsigned dim)
{
	m_terrainXZScale = xzscale;
	m_terrainYMax = ymax;
	m_terrainDim = dim;
	m_terrainHeights = heights;
}