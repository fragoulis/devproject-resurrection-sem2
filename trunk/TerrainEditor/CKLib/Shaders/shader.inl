// ctor

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

inline std::vector<Uniform>& ShaderObject :: uniform()
{return _uniform;}

inline const std::vector<VertexAttribute *>& ShaderObject :: vertex_attrib() const
{return _vertex_attrib;}

inline std::vector<unsigned>& ShaderObject :: attrib_location()
{return _attrib_location;}

inline std::vector<std::string>& ShaderObject :: attrib_nis()
{return _attrib_nis;}

inline const std::vector<Uniform>& ShaderObject :: uniform() const
{return _uniform;}

inline Uniform& ShaderObject :: getUniform(const char * uniname)
{
	std::vector<Uniform> ::iterator it = _uniform.begin();
	while(it != _uniform.end())
	{
		if(!strcmp(uniname,it->nameInShader().c_str()))
		{
			return *it;
		}
		++it;
	}
	ASSERT(0);
	return _uniform[0];
}


