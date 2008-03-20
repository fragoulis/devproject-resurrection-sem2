//*****************************************************************************
//**
//**   File:               PS_Base.h
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-14 18:16:29 +0000 (Fri, 14 Mar 2008) $
//**   Last Revision:      $Revision: 171 $
//**
//*****************************************************************************

#pragma once

#include <string>
#include "../Math/CoordinateFrame.h"
#include "../gfx/VBO/VBO.H"

/*
	The particle system base class.
	Holds all the necessary PS variables.

*/

class Model;
class PS_Base
{
public :
	const std::string& getName() const {return m_nameId;}

	virtual void render() const	=0;					
	virtual void update(const float delta)=0;
	virtual void reset() =0;						
	virtual PS_Base * clone() const =0;		

	void setTransform(const CoordinateFrame& transform);


	PS_Base(const std::string& name,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const bool deleteModelOnDestruction);			//Creates the ps with it's default parameters

	virtual ~PS_Base() = 0;

	const float &getCurrentTime() const { return m_currentTime; };
	const float &getSystemLife() const { return m_systemLife; };

protected :

	std::string   m_nameId;			// name of the particle system
	VAttribStatus m_usedAttribs;	// used attribs by the system, hard-coded in ctor as the _generateData

	// Non - volatile ( most of them used in cloning)

	float		  m_particleSize;
	float		  m_systemLife;
	float		  m_particleLife;
	unsigned	  m_particleNum;
	Model		* m_quadArray;
	int			  m_shaderIndex;		//Might this override model's shader index for quadarray sharing??
	CoordinateFrame m_transform;

	// volatile ( most of them used in rendering / reset)
	float		  m_currentTime;

private : 
	bool		 m_deleteModelOnDestruction;

};