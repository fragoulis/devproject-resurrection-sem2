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