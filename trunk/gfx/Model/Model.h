//*****************************************************************************
//**
//**   File:               Model.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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