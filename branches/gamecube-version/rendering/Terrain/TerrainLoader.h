#ifndef __TerrainLoader_h__
#define __TerrainLoader_h__

/*
	Dummy loader / simplifier, just to do the job.
	Gets dim & heights
*/

struct TerrainLoader
{
	unsigned dim;
	float * heights;
	float extent;
	TerrainLoader(char * filename,const unsigned reduceFactor);
	~TerrainLoader();
};

#endif