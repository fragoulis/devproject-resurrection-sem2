//*****************************************************************************
//**
//**   File:               ModelMgr.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include <gl/glee.h>
#include "../../utility/Singleton.h"
#include <string>
#include <vector>

class Model;
class ParserSection;
class ModelMgr : public Singleton<ModelMgr>
{
	std::string				m_modelDir;
	std::vector<Model *>	m_modelTemplates;

	Model *					_loadOBJ(const std::string& name,const unsigned usage_hint);	// load the file in the templates & return it
	
public :
	ModelMgr(){}
	~ModelMgr();

	Model * getModel(const std::string& fname,const unsigned usage_hint = GL_STATIC_DRAW);  // Search if it exists, else load
	const std::string& getModelDir() const {return m_modelDir;}

	void init(const ParserSection * parsec);

};