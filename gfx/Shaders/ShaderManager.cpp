//*****************************************************************************
//**
//**   File:               ShaderManager.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "../Shaders/ShaderManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../VBO/VBO.h"
#include "../../gfxutils/ConfParser/ConfParser.h"

using namespace std;

bool ShaderManager :: _init = false;
ShaderManager * ShaderManager :: _instance = 0;

ShaderManager :: ShaderManager(const ParserSection * parsec)
:_active_program(0),
_active_shader_index(-1)
{
	_parseConfiguration(parsec);
}

void ShaderManager :: destroy()
{
	if(_init)
	{
		delete _instance;
		_init = false;
	}
}

void ShaderManager :: init(const ParserSection * parsec)
{
	if(!_init)
	{
		_instance = new ShaderManager(parsec);
		_init = true;
	}
}

ShaderManager :: ~ShaderManager()
{
	for(size_t i=0;i<_shaders.size();++i)
	{
		delete _shaders[i];
	}
	_shaders.clear();
	_vertex_attribs.clear();
}

bool ShaderManager :: loadFile(const std::string& shname,const std::string& vert, const std::string& frag)
{
	// Loading the files
	const string full_vert = _shaderDir + vert;
	const string full_frag = _shaderDir + frag;
	ifstream fvert(full_vert.c_str()),
			 ffrag(full_frag.c_str());

	// Convert them to character streams
	string vert_c,frag_c,data_c;
	char ch;
	while(fvert.get(ch))
		vert_c += ch;
	while(ffrag.get(ch))
		frag_c += ch;

	// close the files
	fvert.close();
	ffrag.close();

	return loadMemory(shname,vert_c,frag_c);
}

bool ShaderManager :: loadMemory(const std::string& shname,
								 const std::string& vert, 
								 const std::string& frag)
{
	_shaders.push_back(new ShaderObject(shname));
	_shaders[_shaders.size() - 1]->loadRaw(vert,frag);
	return (_shaders[_shaders.size() - 1]->ok());
}

void ShaderManager :: loadShaderFile(const std::string& shaderfile)
{
	string vert,frag,shname;
	unsigned found = 0;
	ConfParser parser(shaderfile);
	const ParserSection * parsec = parser.getSection("Shader");
	vert = parsec->getVal("Vertex");
	frag = parsec->getVal("Fragment");
	shname = parsec->getVal("Name");
	assert(vert != "");
	assert(frag != "");
	assert(shname != "");


	bool res = loadFile(shname,vert,frag);
	assert(res);
	// if successful, then parse the data again for attribs / uniforms

	//Use Program
	const unsigned last = unsigned(_shaders.size() - 1);
	begin(shname.c_str());
	assert(glIsProgram(_active_program));


	//Parse the attribute data
	parsec = parser.getSection("Attribute");
	if(parsec)
	{
		vector<const ParserSection *> attnames = parsec->getChildren();
		for(size_t i=0;i<attnames.size();++i)
		{
			string nis = attnames[i]->getName();
			assert(nis != "");
			unsigned j;
			for(j=0;j<_vertex_attribs.size();++j)
			{
				if(_vertex_attribs[j].name() == nis)
				{
					_shaders[last]->vertex_attrib().push_back(&(_vertex_attribs[j]));
					break;
				}
			}
			nis = attnames[i]->getVal("NameInShader");
			assert(nis != "");
			_shaders[last]->attrib_nis().push_back(nis);

			_shaders[last]->attrib_location().push_back(
				glGetAttribLocation(_active_program,
									_shaders[_shaders.size() - 1]->_attrib_nis[_shaders[last]->_attrib_nis.size() - 1].c_str()));
			// Checks
			assert(_shaders[last]->_attrib_nis.size() == _shaders[last]->_attrib_location.size());
			assert(_shaders[last]->_attrib_nis.size() == _shaders[last]->_vertex_attrib.size());
			CHECK_GL_ERROR();
		}
	}

	//Return to fixed functionality
	end();
}

void ShaderManager :: _parseAttribute(const ParserSection * parsec)
{
	VertexAttribute va;
	va.parseConfiguration(parsec);
	for(size_t i=0;i<_vertex_attribs.size();++i)
		if(_vertex_attribs[i].name() == va.name())
			return;
	_vertex_attribs.push_back(va);
}

VertexAttribute * ShaderManager :: vertexAttribute(const std::string& name)
{
	vector<VertexAttribute>::iterator it = _vertex_attribs.begin();
	while(it != _vertex_attribs.end())
	{
		if(it->name() == name)
			return &(*it);
		++it;
	}
	assert(0);
	return static_cast<VertexAttribute *>(0);
}

unsigned ShaderManager :: vertexAttributeIndex(const VertexAttribute * const attrib) const
{
	vector<VertexAttribute>::const_iterator it = _vertex_attribs.begin();
	unsigned i=0;
	while(it != _vertex_attribs.end())
	{
		if(&(*it) == attrib)
			return i;
		++it;
		++i;
	}
	assert(0);
	return 0;
}

void ShaderManager :: begin(const char * shname)
{
	size_t i;
	const size_t size = _shaders.size();
	for(i=0;i<size;++i)
		if(!strcmp(_shaders[i]->name().c_str(),shname))
			break;
	begin(int(i));
}

// Methods

void ShaderManager :: begin(const int& shader_index)
{
	assert(shader_index != _shaders.size());
	if(shader_index != _active_shader_index)
	{
		if(shader_index == -1)
		{
			end();
			return;
		}
		// Invalidate all non built-ins
		if(unsigned(_active_shader_index) < int(_shaders.size()))
		{
			const size_t cust_attrib_num = _shaders[_active_shader_index]->_vertex_attrib.size();
			for(size_t i=0;i<cust_attrib_num;++i)
			{
				unsigned index = vertexAttributeIndex(_shaders[_active_shader_index]->_vertex_attrib[i]);
				if(!_shaders[_active_shader_index]->_vertex_attrib[i]->built_in() && VBO::enabledAttribStatus(index))
				{
					VBO::setEnabledAttribStatus(index,0);
					_shaders[_active_shader_index]->_vertex_attrib[i]->DisableClientArray();
				}
			}
		}
		CHECK_GL_ERROR();
		glUseProgram(_shaders[shader_index]->program());
		CHECK_GL_ERROR();
		_active_program = _shaders[shader_index]->program();
		_active_shader_index = shader_index;

		// Set new locations		
		const size_t cust_attrib_num_new = _shaders[shader_index]->_vertex_attrib.size();
		for(size_t i=0;i<cust_attrib_num_new;++i)
		{
			_shaders[shader_index]->_vertex_attrib[i]->set_loc(_shaders[shader_index]->_attrib_location[i]);
		}		
	}
	CHECK_GL_ERROR();
}

void ShaderManager :: _parseConfiguration(const ParserSection * parsec)
{
	// Parse shader dir

	_shaderDir = parsec->getVal("ShaderDir");

	// Parse all vertex attributes
	vector<const ParserSection *> attribs = parsec->getSection("VertexAttribute")->getChildren();
	for(unsigned j=0;j<attribs.size();++j)
		_parseAttribute(attribs[j]);

	// Load all shader files
	vector<string> shaderfiles = parsec->getValVector("ShaderFiles");
	for(unsigned j=0;j<shaderfiles.size();++j)
		loadShaderFile(_shaderDir + shaderfiles[j]);
}