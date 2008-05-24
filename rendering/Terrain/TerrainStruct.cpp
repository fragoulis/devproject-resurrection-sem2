#include "TerrainStruct.h"
#include "../../control/Gamecube/gc_new_delete.h"

TerrainStruct :: TerrainStruct(const float * heights,
							   const unsigned dim,
							   const unsigned splitFactor,
							   const float mapextent)
:m_chunkData(0),
m_dim(dim),
m_chunksPerDim(0),
m_chunkDatanumPerDim(0),
m_heights(heights),
m_extent(mapextent),
m_terrainData(0)
{
	// set variables
	ASSERTMSG(((m_dim-1)%splitFactor) == 0, "Splitfactor must be so as to split the terrain into equal parts!\n");
	m_chunksPerDim = splitFactor;
	m_chunkDatanumPerDim = 1 + ((m_dim-1) / splitFactor);

	// compute helpers
	const unsigned totalChunks = m_chunksPerDim * m_chunksPerDim;
	const unsigned totalChunkData = m_chunkDatanumPerDim * m_chunkDatanumPerDim;
	const unsigned totalChunkIndices = 2 * ( m_chunkDatanumPerDim + 1 ) * ( m_chunkDatanumPerDim - 1 );
	OSReport("**** TERRAIN STRUCT CREATION ********\n");
	OSReport("Total chunks            : %u\n",totalChunks);
	OSReport("Total chunk vertex data : %u\n",totalChunkData);
	OSReport("Total chunk indices     : %u\n\n",totalChunkIndices);

	// allocate memory
	m_terrainData = (VFMT0 *)OSAlloc(totalChunkData * sizeof(VFMT0)*totalChunks);
	VFMT0::IndexStruct * indices = (VFMT0::IndexStruct *)OSAlloc(totalChunkIndices * sizeof(VFMT0::IndexStruct));
	m_chunkData = new TerrainChunk[totalChunks];

	OSReport("Creating vertices / indices..\n");
	// create the entry data / indices
	const float start_ll[2] = {-m_extent*0.5f,m_extent*0.5f};
	const float chunk_extent = m_extent / m_chunksPerDim;
	const float data_extent = chunk_extent / (m_chunkDatanumPerDim-1);
	OSReport("Chunk extent : %f\n",chunk_extent);
	OSReport("Data extent : %f\n",data_extent);
	OSReport("Start : %f %f\n",start_ll[0], start_ll[1]);
	for(unsigned chunk_hi=0; chunk_hi<m_chunksPerDim; ++chunk_hi)
	{
		for(unsigned chunk_wi=0; chunk_wi<m_chunksPerDim; ++chunk_wi)
		{
			const unsigned data_offset = totalChunkData * (chunk_hi*m_chunksPerDim + chunk_wi);
			
			for(unsigned hi = 0; hi < m_chunkDatanumPerDim; ++hi)
			{
				for(unsigned wi =0 ; wi < m_chunkDatanumPerDim; ++wi)
				{
					// set the data entry
					const unsigned point_x = chunk_wi*(m_chunkDatanumPerDim-1) + wi,
							       point_y = chunk_hi*(m_chunkDatanumPerDim-1) + hi;
							
					const unsigned data_i = data_offset + hi*m_chunkDatanumPerDim + wi;
					ASSERTMSG(data_i < (totalChunkData *totalChunks),"CRAP!\n");
					m_terrainData[data_i].xyz[0] = start_ll[0] + point_x*data_extent;
					m_terrainData[data_i].xyz[1] = heights[point_y*dim + point_x];
					m_terrainData[data_i].xyz[2] = start_ll[1] - point_y*data_extent;
					m_terrainData[data_i].st[0] = (u16)point_x;
					m_terrainData[data_i].st[1] = (u16)((m_dim-1) - point_y);

					//if((chunk_hi == 0) && (chunk_wi == 0))
					//	OSReport("Point(%f,%f)\n",m_terrainData[data_i].xyz[0],m_terrainData[data_i].xyz[2]);
				}
			}			
		}
	}

	unsigned i_index = 0;
	for(unsigned hi = 0; hi < (m_chunkDatanumPerDim-1); ++hi)
	{
		for(unsigned wi =0 ; wi < m_chunkDatanumPerDim; ++wi)
		{
			// set the index entries : order = lower, upper
			const unsigned data_index = hi*m_chunkDatanumPerDim + wi;

			indices[i_index].posi = indices[i_index].texi = (u16)(data_index + m_chunkDatanumPerDim);
			++i_index;

			indices[i_index].posi = indices[i_index].texi = (u16)data_index;
			++i_index;
		}
		if(hi != (m_chunkDatanumPerDim-1))
		{
			// add the 2 degenerates
			const u16 topleft = (hi+2)*m_chunkDatanumPerDim;
			const u16 loright = (hi+1)*m_chunkDatanumPerDim - 1;

			// lo right
			indices[i_index].posi = indices[i_index].texi = loright;
			++i_index;
			
			// top left
			indices[i_index].posi = indices[i_index].texi = topleft;
			++i_index;
			
			
		}
	}

	DCFlushRange(m_terrainData,totalChunkData * sizeof(VFMT0)*totalChunks);
	DCFlushRange(indices,totalChunkIndices * sizeof(VFMT0::IndexStruct));

	OSReport("Building vertex arrays..\n");
	// setup the VAs - flush first
	for(int chunki=totalChunks-1; chunki>=0; --chunki)
	{
		m_chunkData[chunki].chunkData = new VertexArray<VFMT0>();
		m_chunkData[chunki].chunkData->set(&m_terrainData[totalChunkData*chunki],
										   //totalChunkData,
										   GX_TRIANGLESTRIP,
										   (u16)totalChunkIndices,
										   indices);
	}
	
	OSReport("Done!\n");
	OSReport("*************************************\n");

	DCFlushRange(indices,totalChunkIndices * sizeof(VFMT0::IndexStruct)*totalChunks);

	// free useless memory
	OSFree(indices);
}

TerrainStruct :: ~TerrainStruct()
{
	const unsigned totalChunks = m_chunksPerDim * m_chunksPerDim;
	const unsigned totalChunkData = m_chunkDatanumPerDim * m_chunkDatanumPerDim;
	DCFlushRange(m_chunkData,sizeof(TerrainChunk)*totalChunks);
	DCFlushRange(m_terrainData,totalChunkData * sizeof(VFMT0)*totalChunks);

	OSFree(m_terrainData);
	delete []m_chunkData;
}

// call the display lists
void TerrainStruct :: render() const
{
	Mtx tm1; 
	const float scalefactor = 1.0f / float(m_dim);
 
	MTXScale(tm1, scalefactor, scalefactor, scalefactor); 
	 
	GXLoadTexMtxImm(tm1, GX_TEXMTX0, GX_MTX2x4); 
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
	GXSetNumTexGens(1);

	const unsigned totalChunks = m_chunksPerDim * m_chunksPerDim;
	for(unsigned i=0;i<totalChunks;++i)
		m_chunkData[i].chunkData->call();
}

// update the cull status
void TerrainStruct :: update()
{
	// get edges from renderengine
	// Vector2 edges[4];
	// RenderEngine::instance().getWSScreenEdges(edges);
	// cull(edges);
}