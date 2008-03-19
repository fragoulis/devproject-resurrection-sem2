//*****************************************************************************
//**
//**   File:               Terrain.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include <string>

class Model;

class Terrain
{
public:
	Terrain();
	~Terrain();


	float getHeight(float x, float z);	// z is always negative since the terrain volume min & max is :
										// min = 0,0,0
										// max = xzscale * dim, ymax, -xzscale * dim
	
	void fillData(float * heights,const float xzscale,const float ymax,const unsigned dim);

	unsigned getTerrainDim() const {return m_terrainDim;}
	
private:

	// The below stuff consider the terrain heights as a dim*dim array of floats
	float		m_terrainXZScale;		// The scale of the terrain in X & Z axes
	float		m_terrainYMax;			// The maximum value in Y axis
	float	  * m_terrainHeights;		// an array with dim*dim heights, created/destroyed per level load/unload
	unsigned	m_terrainDim;			// the dimension of the terrain
};
