//*****************************************************************************
//**
//**   File:               Model.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include <vector>
#include "MaterialGroup.h"

class VBO;
class Model
{
	std::vector<MaterialGroup> m_matGroup;
	std::string				   m_name;
	VBO *					   m_vbo;

public :
	Model(const std::string& name, VBO * vbo):m_name(name),m_vbo(vbo){}
	
	// Accessors
	const std::string& getName() const {return m_name;}
	const std::vector<MaterialGroup>& getMatGroup() const {return m_matGroup;}
	const VBO * getVBO() const {return m_vbo;}

	// Mutator
	MaterialGroup& matGroup(const unsigned i) {return m_matGroup[i];}
	void addMatGroup(const MaterialGroup& mg) {m_matGroup.push_back(mg);}
};