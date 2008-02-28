#pragma once

#include <glee.h>
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

	void init(const ParserSection * parsec);

};