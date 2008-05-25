#ifndef __TerrainStruct_h__
#define __TerrainStruct_h__

#include <dolphin.h>
#include "../../gfxutils/VA/VertexArray.h"

/*
	Class that gets a pointer to the heights, some build params, & builds the terrain render struct.
	This class will cull unseen chunks and will also create shadow triangles.
*/

class TerrainStruct
{
	struct TerrainChunk
	{
		VertexArray<VFMT0> * chunkData;
		bool			     visible;
		TerrainChunk():chunkData(0),visible(false){}
		~TerrainChunk() {if(chunkData) {delete chunkData;chunkData = 0;visible = true;}}
	};

	TerrainChunk * m_chunkData;
	unsigned	   m_dim;
	unsigned	   m_chunksPerDim;		// total chunks = m_chunksPerDim * m_chunksPerDim
	unsigned	   m_chunkDatanumPerDim;	// chunk data num = m_chunkDatanumPerDim * m_chunkDatanumPerDim
	const float	 * m_heights;
	const float	   m_extent;
	VFMT0		 * m_terrainData;

	unsigned	   m_polynum;


public : 

	TerrainStruct(const float * heights,
				  const unsigned dim,
				  const unsigned splitFactor,
				  const float mapextent);
	~TerrainStruct();

	/*
		getShadowTriangles();
		
		input : transformed quad
		output : triangle list (w/ texcoords, probably VMFT1)
	*/

	/*
		cull(Vector2 edges[4]);
		input : visible map edges
		output : nothing
	*/

	void render() const;
	void update();

	const unsigned getPolynum() const {return m_polynum;}
};

#endif