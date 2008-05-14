#include "Texture3D.h"
#include "../../gfxutils/Misc/utils.h"
#include "../../gfxutils/Misc/defines.h"
#include <string>
#include <windows.h>
#include <gl/glee.h>
#include <GL/glu.h>
#include <iostream>

using namespace std;

Texture3D :: Texture3D(const unsigned w,const unsigned h,const unsigned d,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<MipmapLevel>& texdata,const unsigned target,	
			const std::string& texname,const bool genMipmaps)
:Texture(w,ifmt,pfmt,dtype,target,texname,genMipmaps),_height(h),_depth(d)
{
	_type = TEX3D;
	bind();
	_maxMiplevel = GLuint(texdata.size() - 1);	

	// Check if compressed (ARB)
	if((_internalFormat == GL_COMPRESSED_RGB_ARB) ||
		(_internalFormat == GL_COMPRESSED_RGBA_ARB) ||
		(_internalFormat == GL_COMPRESSED_ALPHA_ARB) ||
		(_internalFormat == GL_COMPRESSED_LUMINANCE_ARB) ||
		(_internalFormat == GL_COMPRESSED_LUMINANCE_ALPHA_ARB) ||
		(_internalFormat == GL_COMPRESSED_INTENSITY_ARB))
	{
		_isCompressed = true;
		_maxMiplevel = unsigned(texdata.size()-1);
		// Load all mipmap levels
		unsigned miplevel = 0;
		while(miplevel <= _maxMiplevel)
		{
			const unsigned char * tmp_data = (unsigned char *)(texdata[miplevel].data);
			const double adjust = 1.0 / pow(2.0,int(miplevel));
			unsigned tmp_w = unsigned(_width * adjust),
					 tmp_h = unsigned(_height * adjust),
					 tmp_d = unsigned(_depth * adjust);
			glCompressedTexImage3D(_target,miplevel,_internalFormat,tmp_w,tmp_h,tmp_d,0,texdata[miplevel].size,tmp_data);
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
			const unsigned char * tmp_data = (unsigned char *)(texdata[miplevel].data);
			const double adjust = 1.0 / pow(2.0,int(miplevel));
			unsigned tmp_w = unsigned(_width * adjust),
					 tmp_h = unsigned(_height * adjust),
					 tmp_d = unsigned(_depth * adjust);
			glTexImage3D(_target,miplevel,_internalFormat,tmp_w,tmp_h,tmp_d,0,_pixelFormat,_datatype,tmp_data);
			++miplevel;
		}
		glTexParameteri(_target,GL_GENERATE_MIPMAP,GL_FALSE);
		if(genMipmaps)
			_maxMiplevel = GLuint(math_const<float>::log2(float(MAX(MAX(_width,_height),_depth))));
	}

	setParam(GL_TEXTURE_WRAP_S,GL_REPEAT);
	setParam(GL_TEXTURE_WRAP_T,GL_REPEAT);
	setParam(GL_TEXTURE_WRAP_R,GL_REPEAT);
	if(_maxMiplevel)
		setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	else
		setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	setParam(GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	// Compute bytes per texel

	GLint vals[6];
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_RED_SIZE,&vals[0]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_GREEN_SIZE,&vals[1]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_BLUE_SIZE,&vals[2]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_ALPHA_SIZE,&vals[3]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_LUMINANCE_SIZE,&vals[4]);
	glGetTexLevelParameteriv(_target,0,GL_TEXTURE_INTENSITY_SIZE,&vals[5]);
	_bytesPerTexel += vals[0];
	_bytesPerTexel += vals[1];
	_bytesPerTexel += vals[2];
	_bytesPerTexel += vals[3];
	_bytesPerTexel += vals[4];
	_bytesPerTexel += vals[5];
	_bytesPerTexel >>= 3;
}

const GLuint Texture3D :: dataSize(const int miplevel) const
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
		size = GLuint(_width*_height*_depth*adjust*adjust*adjust*_bytesPerTexel);
	}
	return size;
}

void Texture3D :: ulData(const void * buffer,const GLuint miplevel) const
{
	bind();
	assert(!_isCompressed);
	glTexSubImage3D(_target,miplevel,0,0,0,_width,_height,_depth,_internalFormat,_datatype,buffer);
}

void Texture3D :: dlData(void * buffer,const GLuint miplevel) const
{
	bind();
	if(_isCompressed)
	{
		glGetCompressedTexImage(_target,miplevel,buffer);
	}
	else
		glGetTexImage(_target,miplevel,_pixelFormat,_datatype,buffer);
}

TEX_header Texture3D :: getTEXheader() const
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
	header.depth = _depth;
	return header;
}