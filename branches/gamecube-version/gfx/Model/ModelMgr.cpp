#include "ModelMgr.h"
#include "Model.h"
#include "../../gfxutils/Misc/utils.h"
#include "../../gfxutils/ConfParser/ParserSection.h"

using namespace std;

void ModelMgr :: init(const ParserSection * parsec)
{
	m_modelDir = parsec->getVal("ModelDir");
	vector<string> modnames = parsec->getValVector("PersistentModels");
	for(size_t i=0;i<modnames.size();++i)
		getModel(modnames[i]);

}

ModelMgr :: ~ModelMgr()
{
	for(std::vector<Model *>::iterator it = m_modelTemplates.begin();
		it != m_modelTemplates.end();
		++it)
	{
		delete (*it);
	}
	m_modelTemplates.clear();
}

Model * ModelMgr :: getModel(const std::string& name,const unsigned usage_hint)
{
	// Search for the file in model Templates
	for(std::vector<Model *>::iterator it = m_modelTemplates.begin();
		it != m_modelTemplates.end();
		++it)
	{
		if((*it)->getName() == name)
			return (*it);
	}
	
	// We haven't found it, so load it, first appending the dir
	const std::string fullname = m_modelDir + name;
	Model * model = _loadOBJ(fullname,usage_hint);
	m_modelTemplates.push_back(model);
	return model;
}
