#ifndef _Shader_h_
#define _Shader_h_

#include <vector>
#include <string>
#include <glee.h>
#include <Misc/defines.h>
#include <Misc/utils.h>
#include <Shaders/VertexAttrib.h>
#include <Shaders/Uniform.h>

namespace CKLib {

/*
	Shader :
	Shader program, either a vertex or a fragment
*/
class Shader
{
	GLuint _object;

public : 

	// ctor / dtor
	Shader();
	~Shader();

	//funcs
	bool compile(const std::string& str);
	const std::string getCompilerLog();
	const GLuint& object() const;
	GLuint& object();

};

/*
	ShaderObject : 
	Shader container , but ONLY one pair of vertex/ fragment programs
*/
class ShaderObject
{
	GLuint		_programID;
	Shader		_vert_shader,
				_frag_shader;

	bool		_all_ok;
	std::string _name;

	std::vector<VertexAttribute *>	_vertex_attrib;			// custom attributes used
	std::vector<unsigned>			_attrib_location;		// custom attribute locations
	std::vector<std::string>		_attrib_nis;			// Custom Attribute names

	std::vector<Uniform>			_uniform;				// uniforms used

	friend class ShaderManager;

public : 

	// ctor / dtor
	ShaderObject(const std::string& name);
	ShaderObject();
	~ShaderObject();
	
	// Methods
	void loadRaw(const std::string& vert, const std::string& frag);

	// Access
	bool ok();	// Status of shader object
	const std::string& name() const;
	const GLuint& program() const;
	std::vector<VertexAttribute *>& vertex_attrib();
	std::vector<unsigned>& attrib_location();
	std::vector<std::string>& attrib_nis();
	std::vector<Uniform>& uniform();
	const std::vector<VertexAttribute *>& vertex_attrib() const;
	const std::vector<unsigned>& attrib_location() const;
	const std::vector<std::string>& attrib_nis() const;
	const std::vector<Uniform>& uniform() const;

	bool link();
	const std::string getLinkerLog();
	Uniform& getUniform(const char * uniname);

};

#include <Shaders/Shader.inl>

}

#endif