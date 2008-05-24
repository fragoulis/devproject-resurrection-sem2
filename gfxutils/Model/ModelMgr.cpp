#include "ModelMgr.h"
#include "Model.h"
#include "../../control/Gamecube/gc_new_delete.h"
	
ModelMgr :: ModelMgr()
{
	
}

const Model * ModelMgr :: getModel(const std::string& name)
{
	OSReport("Requesting model %s\n",name.c_str());
	for(unsigned i=0;i<m_modelList.size();++i)
	{
		if(name == m_modelList[i]->getName())
			return m_modelList[i];
	}
	// if m_modelNum == MAX_MODELS, assert!!!!

	// not found, so load it from file
	m_modelList.push_back(new Model(name));
	return m_modelList.back();
}

void ModelMgr :: unloadModel(const Model * model)
{
	for(unsigned i=0;i<m_modelList.size();++i)
	{
		if(model == m_modelList[i])
		{
			delete m_modelList[i];
			m_modelList[i] = m_modelList.back();
			m_modelList.pop_back();
			return;
		}
	}
}

void ModelMgr :: unloadModel(const std::string& name)
{
	for(unsigned i=0;i<m_modelList.size();++i)
	{
		if(name == m_modelList[i]->getName())
		{
			delete m_modelList[i];
			m_modelList[i] = m_modelList.back();
			m_modelList.pop_back();
			return;
		}
	}
}

ModelMgr :: ~ModelMgr()
{
	for(unsigned i=0;i<m_modelList.size();++i)
	{
		delete m_modelList[i];
		m_modelList[i] = 0;
	}
	m_modelList.clear();
}

void ModelMgr :: init(const ParserSection * parsec)
{
	m_modelDir = parsec->getVal("ModelDir");
	OSReport("Model directory set to %s...\n",m_modelDir.c_str());
}