//*****************************************************************************
//**
//**   File:               Shader.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once

#include <vector>
#include <string>
#include <gl/glee.h>
#include "../../gfxutils/Misc/defines.h"
#include "../../gfxutils/Misc/utils.h"
#include "../Shaders/VertexAttrib.h"

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
	const std::vector<VertexAttribute *>& vertex_attrib() const;
	const std::vector<unsigned>& attrib_location() const;
	const std::vector<std::string>& attrib_nis() const;

	bool link();
	const std::string getLinkerLog();

};

inline ShaderObject :: ShaderObject(const std::string& name)
:_name(name),
_programID(0)
{
}

inline ShaderObject :: ShaderObject()
:_programID(0)
{
}

inline Shader :: Shader()
{
	_object = 0;
}

// Access
inline bool ShaderObject :: ok()
{return _all_ok;}

inline const std::string& ShaderObject :: name() const
{return _name;}

inline const GLuint& Shader :: object() const
{return _object;}

inline GLuint& Shader :: object()
{return _object;}

inline const GLuint& ShaderObject :: program() const
{return _programID;}

inline std::vector<VertexAttribute *>& ShaderObject :: vertex_attrib()
{return _vertex_attrib;}

inline const std::vector<unsigned>& ShaderObject :: attrib_location() const
{return _attrib_location;}

inline const std::vector<std::string>& ShaderObject :: attrib_nis() const
{return _attrib_nis;}

inline const std::vector<VertexAttribute *>& ShaderObject :: vertex_attrib() const
{return _vertex_attrib;}

inline std::vector<unsigned>& ShaderObject :: attrib_location()
{return _attrib_location;}

inline std::vector<std::string>& ShaderObject :: attrib_nis()
{return _attrib_nis;}



