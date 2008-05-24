#ifndef __ModelMgr_h__
#define __ModelMgr_h__

#include "../../utility/Singleton.h"
#include "../confparser/confparser.h"
#include <string>

class Model;
class ModelMgr : public Singleton<ModelMgr>
{
	std::vector<Model *> m_modelList;
	std::string m_modelDir;

	Model * _loadModelFile(const std::string& name);

public : 
	
	ModelMgr();
	~ModelMgr();
	const Model * getModel(const std::string& name);
	void unloadModel(const Model * model);
	void unloadModel(const std::string& name);
	void init(const ParserSection * parsec);
	const std::string& getModelDir() const {return m_modelDir;};

};

#endif