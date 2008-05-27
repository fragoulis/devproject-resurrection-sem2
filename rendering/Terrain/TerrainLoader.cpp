#include <dolphin.h>
#include "TerrainLoader.h"
#include "../../utility/RawFile.h"

template <class T>
void swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
};

static float swapf(float value)
{
	char * tmp = (char *)(&value);	
	swap(tmp[0],tmp[3]);
	swap(tmp[1],tmp[2]);
	return value;
}

static unsigned swapu(unsigned value)
{
	char * tmp = (char *)(&value);	
	swap(tmp[0],tmp[3]);
	swap(tmp[1],tmp[2]);
	return value;
}


TerrainLoader :: TerrainLoader(char * filename,const unsigned reduceFactor)
:dim(0),heights(0),extent(0.0f)
{
	RawFile mapfile(filename);
	ASSERTMSG(mapfile.data,"Data file gone wrong!\n");
	dim = swapu(*((unsigned *)mapfile.data));
	
	const unsigned elems = dim * dim;
	float * mapdata = (float *)(&((unsigned *)mapfile.data)[1]);
	heights = (float *)OSAlloc(elems*sizeof(float));
	unsigned out_i = 0;
	unsigned peekPoint = 1<<reduceFactor;
	for(unsigned i=0;i<dim;i+=peekPoint)
	{
		for(unsigned j=0;j<dim;j+=peekPoint)
		{
			heights[out_i] = swapf(mapdata[((i*dim + j)<<2) + 1]);
			++out_i;
		}
	}
	extent = swapf(mapdata[(dim-1)<<2]) - (swapf(mapdata[0]));		// FIXME : temporary!

	dim = ((dim - 1) / peekPoint) + 1;

	//OSReport("Loaded map with dim = %u, extent = %f\n",dim,extent);
}

TerrainLoader :: ~TerrainLoader()
{
	if(heights)
		OSFree(heights);
}