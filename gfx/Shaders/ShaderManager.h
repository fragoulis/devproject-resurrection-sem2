//*****************************************************************************
//**
//**   File:               ShaderManager.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once

#include <vector>
#include <string>
#include "../Shaders/Shader.h"
#include "../Shaders/VertexAttrib.h"
#include <gl/glee.h>

/*
	ShaderManager : 
	Manages all the shader usage in the application
	holds all the shaders
	changes programs used
	loads .sh files 
	manages uniforms & vertex attributes
*/
class ParserSection;
class ShaderManager
{
	std::string						_shaderDir;
	std::vector<ShaderObject *>		_shaders;
	std::vector<VertexAttribute>	_vertex_attribs;
	GLuint							_active_program;
	GLint							_active_shader_index;

	static bool			   _init;
	static ShaderManager * _instance;

	ShaderManager(const ParserSection * parsec);

	void _parseConfiguration(const ParserSection * parsec);

	void _parseAttribute(const ParserSection * parsec);

	// Load vertex shader, fragment shader of named shader
	bool loadFile(const std::string& shname,
				  const std::string& vert, 
				  const std::string& frag);
	// Load vertex shader, fragment shader of named shader, as character streams
	bool loadMemory(const std::string& shname,
					const std::string& vert, 
					const std::string& frag);
	
	void setActiveProgram(const unsigned prog);

public : 

	// accessors & dtor
	static ShaderManager * instance() {return _instance;}
	~ShaderManager();
	static void init(const ParserSection * parsec);
	static void destroy();

	ShaderObject * shader(const unsigned i);

	// utils
	const std::string& getShaderDir() const;

	// Parses the shader file for the v & f shaders & associated data
	void loadShaderFile(const std::string& shaderfile);
	// Parses the list of used shaders from the configuration file

	int getShaderIndex(const std::string& name);
	const std::string& getShaderName(const size_t index);

	const unsigned currentActiveProgram() const;
	void begin(const char * shname);		// by name of shader
	void begin(const int& shader_index);		// by index in shadermanager
	void end();

//	void setUniform(const char * nis, const void * mem); - deprecated
	void setUniform1fv(const char * nis, const GLfloat * mem);
	void setUniform2fv(const char * nis, const GLfloat * mem);
	void setUniform3fv(const char * nis, const GLfloat * mem);
	void setUniform4fv(const char * nis, const GLfloat * mem);
	void setUniform1iv(const char * nis, const GLint * mem);
	void setUniform2iv(const char * nis, const GLint * mem);
	void setUniform3iv(const char * nis, const GLint * mem);
	void setUniform4iv(const char * nis, const GLint * mem);
	void setUniform1i(const char * nis, const GLint val);
	void setUniformMatrix4fv(const char * nis, const GLfloat * mem);

	VertexAttribute * vertexAttribute(const std::string& name);
	VertexAttribute * vertexAttribute(const unsigned index);
	size_t vertexAttributeNum() const;
	unsigned vertexAttributeIndex(const VertexAttribute * const attrib) const;
};

inline ShaderObject * ShaderManager :: shader(const unsigned i)
{
	ASSERT(i < _shaders.size());
	return _shaders[i];
}

inline void ShaderManager :: setActiveProgram(const unsigned prog)
{_active_program = prog;}

inline const unsigned ShaderManager :: currentActiveProgram() const
{return _active_program;}

inline int ShaderManager :: getShaderIndex(const std::string& name)
{
	const size_t size = _shaders.size();
	for(unsigned i=0;i<size;++i)
	{
		if(!strcmp(_shaders[i]->name().c_str(),name.c_str()))
			return i;
	}
	return -1;
}

inline const std::string& ShaderManager :: getShaderName(const size_t index)
{
	ASSERT((index > 0) && (index < _shaders.size()));
	return _shaders[index]->name();
}

inline void ShaderManager :: end()
{
	_active_program = 0;
	_active_shader_index = -1;
	CHECK_GL_ERROR();
	glUseProgram(0);
}

inline const std::string& ShaderManager :: getShaderDir() const {return _shaderDir;}

/* DEPRECATED!! - SLOOOOOOOOOOOOOOOOOOOOOOOWWWWWW */
/*
inline void ShaderManager :: setUniform(const char * nis, const void * mem)
{
	if(_active_shader_index < 0)
		return;
	_shaders[_active_shader_index]->getUniform(nis).function()->setUniform(mem);
	return;
}
*/

inline VertexAttribute * ShaderManager :: vertexAttribute(const unsigned index)
{
	ASSERT(index < _vertex_attribs.size());
	return &(_vertex_attribs[index]);
}

inline size_t ShaderManager :: vertexAttributeNum() const
{
	return _vertex_attribs.size();
}

inline void ShaderManager :: setUniform1fv(const char * nis, const GLfloat * mem)
{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform1fv(loc,1,mem);
	
}

inline void ShaderManager :: setUniform2fv(const char * nis, const GLfloat * mem)
{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform2fv(loc,1,mem);
	
}

inline void ShaderManager :: setUniform3fv(const char * nis, const GLfloat * mem)
{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform3fv(loc,1,mem);
	
}

inline void ShaderManager :: setUniform4fv(const char * nis, const GLfloat * mem)

{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform4fv(loc,1,mem);
	
}

inline void ShaderManager :: setUniform1iv(const char * nis, const GLint * mem)

{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform1iv(loc,1,mem);
	
}

inline void ShaderManager :: setUniform1i(const char * nis, const GLint val)

{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform1i(loc,val);
	
}

inline void ShaderManager :: setUniform2iv(const char * nis, const GLint * mem)
{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform2iv(loc,1,mem);
	
}

inline void ShaderManager :: setUniform3iv(const char * nis, const GLint * mem)
{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform3iv(loc,1,mem);
	
}

inline void ShaderManager :: setUniform4iv(const char * nis, const GLint * mem)
{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniform4iv(loc,1,mem);
}

inline void ShaderManager :: setUniformMatrix4fv(const char * nis, const GLfloat * mem)
{
	// get the location
	const int loc = glGetUniformLocation(_active_program,nis);
	glUniformMatrix4fv(loc,1,GL_FALSE,mem);
}


