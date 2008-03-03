#pragma once

#include <string>

/*
	The particle system base class.
	Holds all the necessary PS variables.

*/

class Model;
class PS_Base
{
public :
	const std::string& getName() const {return m_nameId;}

	PS_Base(const std::string& name);
	virtual ~PS_Base(){}
	virtual void update(){}					// fetch the current time & update uniforms

protected :

	std::string   m_nameId;
	// Non - volatile

	float		  m_particleSize;
	float		  m_systemLife;
	float		  m_particleLife;
	Model		* m_quadArray;

	// volatile
	float		  m_currentTime;

};