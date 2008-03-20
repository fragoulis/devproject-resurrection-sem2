//*****************************************************************************
//**
//**   File:               Shader.cpp
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-11 23:17:14 +0000 (Tue, 11 Mar 2008) $
//**   Last Revision:      $Revision: 143 $
//**
//*****************************************************************************

#include "../Shaders/Shader.h"
#include "../../gfxutils/Misc/utils.h"
#include <string>
#include <iostream>

using namespace std;

void ShaderObject :: loadRaw(const std::string& vert, const std::string& frag)
{
	CHECK_GL_ERROR();
	_vert_shader.object() = glCreateShader(GL_VERTEX_SHADER);
	_frag_shader.object() = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned error = 0;
	_programID = glCreateProgram();
	error += _vert_shader.compile(vert) ? 0 : 1;

	string clog = _vert_shader.getCompilerLog();
	if(clog != "")
	{
		cerr<<"Vertex Shader Compiler Log  : "<<endl;
		cerr<<clog<<endl;
	}
	if(error)	return;
	error += _frag_shader.compile(frag) ? 0 : 1;

	clog = _frag_shader.getCompilerLog();
	if(clog != "")
	{
		cerr<<"Fragment Shader Compiler Log  : "<<endl;
		cerr<<clog<<endl;
	}
	if(error)	return;
	error += link() ? 0 : 1;

	string llog = getLinkerLog();
	if(llog != "")
	{
		cerr<<"Program Linker Log : "<<endl;
		cerr<<llog<<endl;
	}
	_all_ok = error ? false : true;
	CHECK_GL_ERROR();
}

Shader :: ~Shader()
{	
	if(_object)
	glDeleteShader(_object);
}

ShaderObject :: ~ShaderObject()
{
	if(_programID)
	{
		if(_vert_shader.object())
			glDetachShader(_programID,_vert_shader.object());
		if(_frag_shader.object())
			glDetachShader(_programID,_frag_shader.object());
		glDeleteProgram(_programID);
	}
}

bool Shader :: compile(const std::string &str) 
{
  GLint	length = 0;
  const char * cstr = str.c_str();
  vector<GLint> lengths;
  vector<const char *> addrs;
  unsigned lines = 1,incr = 0;
  addrs.push_back(cstr);
  while(cstr[incr] != 0)
  {
	if(cstr[incr] == '\n')
	{
		if(cstr[incr+1] != 0)
		{
			++lines;
			addrs.push_back(&cstr[incr+1]);
		}
		lengths.push_back(length);
		length = 0;
	}
	++incr;
	++length;
  }
  lengths.push_back(length);
  glShaderSource(_object, lines, &(addrs[0]), &(lengths[0]));
  CHECK_GL_ERROR();

  glCompileShader(_object); 
  CHECK_GL_ERROR();
  GLint compiled;
  glGetShaderiv(_object, GL_COMPILE_STATUS, &compiled);
  CHECK_GL_ERROR();

  return (compiled)?true : false;
}

const std::string Shader::getCompilerLog(void)
{     
	int len = 0,slen;	
	glGetShaderiv(_object, GL_INFO_LOG_LENGTH , &len);
	CHECK_GL_ERROR();
	if(!len)
		return string("");

	char * compiler_log = new char[len];
	compiler_log[0] = '\0';
	if (len > 1)
	{
		 glGetShaderInfoLog(_object, len, &slen, compiler_log);
		 CHECK_GL_ERROR();     
	}
	string log;
	if(compiler_log[0] != '\0')
		log = string(compiler_log);
	delete []compiler_log;
	return log;
}

const std::string ShaderObject::getLinkerLog(void)
{     
	int len = 0,slen;	
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH , &len);
	CHECK_GL_ERROR();
	if(!len)
		return string("");

	char * linker_log = new char[len];
	linker_log[0] = '\0';
	if (len > 1)
	{
		 glGetProgramInfoLog(_programID, len, &slen, linker_log);
		 CHECK_GL_ERROR();     
	}
	string log;
	if(linker_log[0] != '\0')
		log = string(linker_log);
	delete []linker_log;
	return log;
}

bool ShaderObject::link(void)
{ 
	glAttachShader(_programID, _vert_shader.object());
	CHECK_GL_ERROR();
	glAttachShader(_programID, _frag_shader.object());
	CHECK_GL_ERROR();

	int linked;
	glLinkProgram(_programID);
	CHECK_GL_ERROR();
	glGetProgramiv(_programID, GL_LINK_STATUS, &linked);
	CHECK_GL_ERROR();

	return linked ? true : false;
}
