#ifndef _ShaderManager_h_
#define _ShaderManager_h_

#include <vector>
#include <string>
#include <Shaders/Shader.h>
#include <Shaders/VertexAttrib.h>
#include <Shaders/Uniform.h>
#include <glee.h>

namespace CKLib {

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

#include <Shaders/ShaderManager.inl>

}

#endif