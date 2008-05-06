#include <VBO/VBOMgr.h>
#include <ConfParser/ParserSection.h>
#include <Misc/utils.h>
#include <VBO/VBO.h>
#include <Shaders/VertexAttrib.h>

using namespace CKLib;
using namespace std;

bool VBOMgr :: _init(NULL);
VBOMgr * VBOMgr :: _instance(NULL);

VBOMgr :: VBOMgr(const ParserSection * parsec)
:_currentConfig(-1),
_enabledAttribs(0xFFFFFFFF)
{
	_parseConfiguration(parsec);
}

VBOMgr :: ~VBOMgr()
{
	for(size_t i=0;i<_vboList.size();++i)
		delete _vboList[i];
}

void VBOMgr :: setActiveConfig(const std::string& configname)
{
	for(size_t i =0; i<_configurations.size();++i)
		if(configname == _configurations[i].name)
			_currentConfig = int(i);
}

void VBOMgr :: _parseConfiguration(const ParserSection * parsec)
{
	// Get the various configurations
	std::vector<const ParserSection *> confs = parsec->getChildren();
	for(size_t i=0;i<confs.size();++i)
	{
		VBOConf localconf;
		localconf.name = confs[i]->getName();
		const string usage = confs[i]->getVal("DrawUsage");
		if(usage == "static")
			localconf.drawUsage = GL_STATIC_DRAW;
		else if(usage == "dynamic")
			localconf.drawUsage = GL_DYNAMIC_DRAW;
		else if(usage == "stream")
			localconf.drawUsage = GL_STREAM_DRAW;
		else
			assert(0);
		localconf.vertexData = FromString<unsigned>(confs[i]->getVal("VertexData"));
		localconf.indexData = FromString<unsigned>(confs[i]->getVal("IndexData"));
		_configurations.push_back(localconf);
	}
}

void VBOMgr :: init(const ParserSection * parsec)
{
	if(!_init)
	{
		_instance = new VBOMgr(parsec);
		_init = true;
	}
}

void VBOMgr :: destroy()
{
	if(_init)
	{
		delete _instance;
		_init = false;
	}
}

void VBOMgr :: getActiveConfig(std::string& configname)
{
	assert(_currentConfig);
	configname = _configurations[_currentConfig].name;
}

VBO * VBOMgr :: fetchVBO(std::vector<const VertexAttribute *> attribs,
						   const unsigned maxVertices,
						   const unsigned maxIndices)
{
	assert(_currentConfig >= 0);
	// With the given reqs & the active config, fetch or create & fetch a vbo

	// Assert we don't ask too much data
	assert(maxVertices <= _configurations[_currentConfig].vertexData);
	assert(maxIndices <= _configurations[_currentConfig].indexData);

	// Create a VBO ptr & vert & ind offsets
	unsigned voffset(0),
			 ioffset(0);
	VBO * vbo = NULL;
	bool ok(false);

	for(size_t i=0;i< _vboList.size();++i)
	{
		vbo = _vboList[i];
		// Check usage
		if(vbo->drawUsage() == _configurations[_currentConfig].drawUsage)
		{
			unsigned found(0);
			for(unsigned j=0;j<attribs.size();++j)
				for(unsigned k=0;k<vbo->attrib_num();++k)
					if(vbo->attrib(k) == attribs[j])
						++found;
			if((found == attribs.size()) && 
			   (found == vbo->attrib_num()) && 
				vbo->querySize(maxVertices,maxIndices))
			{
				ok = true;
				i = _vboList.size();
				break;
			}
		}
	}
	// If no matching VBO found, create a new one
	if(!ok)
	{
		//create based on characteristics
		vbo = new VBO(attribs,
					  _configurations[_currentConfig].vertexData,
					  _configurations[_currentConfig].indexData,
					  _configurations[_currentConfig].drawUsage,
					  _configurations[_currentConfig].name);
		_vboList.push_back(vbo);
	}
	return vbo;
}