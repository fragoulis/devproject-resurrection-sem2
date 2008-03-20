//*****************************************************************************
//**
//**   File:               Texture1D.cpp
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-19 20:39:19 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 200 $
//**
//*****************************************************************************

#include "Texture1D.h"
#include "../../gfxutils/Misc/utils.h"
#include "../../gfxutils/Misc/defines.h"
#include <string>
#include <windows.h>
#include <gl/glee.h>
#include <GL/glu.h>
#include <iostream>

using namespace std;

Texture1D :: Texture1D(const unsigned w,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<MipmapLevel>& texdata,const unsigned target,	
			const std::string& texname,const bool genMipmaps,
			const bool loadAsRect)
:Texture(w,ifmt,pfmt,dtype,target,texname,genMipmaps)
{
	
	_type = TEX1D;
	bind();
	CHECK_GL_ERROR();
	_maxMiplevel = GLuint(texdata.size() - 1);


	_isCompressed = false;	
	// Load all mipmap levels
	if(genMipmaps)
		glTexParameteri(_target,GL_GENERATE_MIPMAP,GL_TRUE);
	unsigned miplevel = 0;
	while(miplevel <= _maxMiplevel)
	{
		const unsigned char * tmp_data = (unsigned char *)(texdata[miplevel].data);
		const double adjust = 1.0 / pow(2.0,int(miplevel));
		unsigned tmp_w = unsigned(_width * adjust);
		glTexImage1D(_target,miplevel,_internalFormat,tmp_w,0,_pixelFormat,_datatype,tmp_data);
		++miplevel;
	}

	glTexParameteri(_target,GL_GENERATE_MIPMAP,GL_FALSE);
	CHECK_GL_ERROR();

	setParam(GL_TEXTURE_WRAP_S,GL_REPEAT);
	setParam(GL_TEXTURE_WRAP_T,GL_REPEAT);
	if(_maxMiplevel)
		setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	else
		setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	setParam(GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	CHECK_GL_ERROR();

	// Compute bytes per texel

	GLint vals[7];
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_RED_SIZE,&vals[0]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_GREEN_SIZE,&vals[1]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_BLUE_SIZE,&vals[2]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_ALPHA_SIZE,&vals[3]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_LUMINANCE_SIZE,&vals[4]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_INTENSITY_SIZE,&vals[5]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_DEPTH_SIZE,&vals[6]);
	_bytesPerTexel += vals[0];
	_bytesPerTexel += vals[1];
	_bytesPerTexel += vals[2];
	_bytesPerTexel += vals[3];
	_bytesPerTexel += vals[4];
	_bytesPerTexel += vals[5];
	_bytesPerTexel += vals[6];
	_bytesPerTexel >>= 3;
}

const GLuint Texture1D :: dataSize(const int miplevel) const
{
	bind();
	GLint size = 0;	
	const double adjust = 1.0 / pow(2.0,miplevel);
	size = GLuint(_width*adjust*_bytesPerTexel);
	return size;
}

void Texture1D :: ulData(const void * buffer,const GLuint miplevel) const
{
	bind();
	assert(!_isCompressed);
	glTexSubImage1D(_target,miplevel,0,_width,_internalFormat,_datatype,buffer);
}

void Texture1D :: dlData(void * buffer,const GLuint miplevel) const
{
	bind();
	glGetTexImage(_target,miplevel,_pixelFormat,_datatype,buffer);
}

TEX_header Texture1D :: getTEXheader() const
{
	TEX_header header;
	header.compressed = _isCompressed;
	header.datatype = _datatype;
	header.height = 0;
	header.ifmt = _internalFormat;
	header.pfmt = _pixelFormat;
	header.magic = 1234;
	header.mipcount = _maxMiplevel +1;
	header.textype = _type;
	header.width = _width;	
	return header;
}