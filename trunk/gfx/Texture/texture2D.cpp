//*****************************************************************************
//**
//**   File:               texture2D.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "Texture2D.h"
#include "../../gfxutils/Misc/utils.h"
#include "../../gfxutils/Misc/defines.h"
#include <string>
#include <windows.h>
#include <gl/glee.h>
#include <GL/glu.h>
#include <iostream>

using namespace std;

Texture2D :: Texture2D(const unsigned w,const unsigned h,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<MipmapLevel>& texdata,const unsigned target,	
			const std::string& texname,const bool genMipmaps,
			const bool loadAsRect)
:Texture(w,ifmt,pfmt,dtype,target,texname,genMipmaps),_height(h)
{
	
	_type = TEX2D;
	bind();
	CHECK_GL_ERROR();
	_maxMiplevel = (loadAsRect) ? 0 : GLuint(texdata.size() - 1);

	// Check if compressed
	if((_internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT) ||
		(_internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ||
		(_internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) ||
		(_internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT))
	{
		_isCompressed = true;
		// Load all mipmap levels
		unsigned miplevel = 0;
		while(miplevel <= _maxMiplevel)
		{
			const unsigned char * tmp_data = texdata[miplevel].data;
			const double adjust = 1.0 / pow(2.0,int(miplevel));
			unsigned tmp_w = unsigned(_width * adjust),
					 tmp_h = unsigned(_height * adjust);
			glCompressedTexImage2D(_target,miplevel,_internalFormat,tmp_w,tmp_h,0,texdata[miplevel].size,tmp_data);
			++miplevel;
		}
		
	}
	else
	{
		_isCompressed = false;	
		// Load all mipmap levels
		if(genMipmaps)
			glTexParameteri(_target,GL_GENERATE_MIPMAP,GL_TRUE);
		unsigned miplevel = 0;
		while(miplevel <= _maxMiplevel)
		{
			const unsigned char * tmp_data = texdata[miplevel].data;
			const double adjust = 1.0 / pow(2.0,int(miplevel));
			unsigned tmp_w = unsigned(_width * adjust),
					 tmp_h = unsigned(_height * adjust);
			glTexImage2D(_target,miplevel,_internalFormat,tmp_w,tmp_h,0,_pixelFormat,_datatype,tmp_data);
			++miplevel;
		}
		if(!loadAsRect)
			glTexParameteri(_target,GL_GENERATE_MIPMAP,GL_FALSE);
		CHECK_GL_ERROR();
	}
	if(loadAsRect)
	{
		setParam(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		setParam(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		setParam(GL_TEXTURE_MIN_FILTER,GL_NEAREST);	
		setParam(GL_TEXTURE_MAG_FILTER,GL_NEAREST);	
	}
	else
	{
		setParam(GL_TEXTURE_WRAP_S,GL_REPEAT);
		setParam(GL_TEXTURE_WRAP_T,GL_REPEAT);
		if(_maxMiplevel)
			setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		else
			setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		setParam(GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	}
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

const GLuint Texture2D :: dataSize(const int miplevel) const
{
	bind();
	GLint size = 0;
	if(_isCompressed)
	{
		glGetTexLevelParameteriv(_target,miplevel,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&size);
	}
	else
	{
		const double adjust = 1.0 / pow(2.0,miplevel);
		size = GLuint(_width*_height*adjust*adjust*_bytesPerTexel);
	}
	return size;
}

void Texture2D :: ulData(const void * buffer,const GLuint miplevel) const
{
	bind();
	assert(!_isCompressed);
	if(_target == GL_TEXTURE_2D)
		glTexSubImage2D(_target,miplevel,0,0,_width,_height,_internalFormat,_datatype,buffer);
	else
		glTexImage2D(_target,miplevel,_internalFormat,_width,_height,0,_pixelFormat,_datatype,buffer);
}

void Texture2D :: dlData(void * buffer,const GLuint miplevel) const
{
	bind();
	if(_isCompressed)
	{
		glGetCompressedTexImage(_target,miplevel,buffer);
	}
	else
		glGetTexImage(_target,miplevel,_pixelFormat,_datatype,buffer);
}

TEX_header Texture2D :: getTEXheader() const
{
	TEX_header header;
	header.compressed = _isCompressed;
	header.datatype = _datatype;
	header.height = _height;
	header.ifmt = _internalFormat;
	header.pfmt = _pixelFormat;
	header.magic = 1234;
	header.mipcount = _maxMiplevel +1;
	header.textype = _type;
	header.width = _width;	
	return header;
}