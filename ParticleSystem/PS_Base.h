#pragma once

#include <string>
#include "../Math/CoordinateFrame.h"

/*
	The particle system base class.
	Holds all the necessary PS variables.

*/

class Model;
class PS_Base
{
public :
	const std::string& getName() const {return m_nameId;}

	virtual void render(const float delta);		// fetch the current time & update uniforms
	virtual void reset();

	void setTransform(const CoordinateFrame& transform);


	PS_Base(const std::string& name,
			const Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const int shindex);			//Creates the ps with it's default parameters

	virtual ~PS_Base(){}

protected :

	std::string   m_nameId;
	// Non - volatile

	float		  m_particleSize;
	float		  m_systemLife;
	float		  m_particleLife;
	const Model	* m_quadArray;
	int			  m_shaderIndex;		//Might this override model's shader index for quadarray sharing??
	CoordinateFrame m_transform;

	// volatile
	float		  m_currentTime;

};