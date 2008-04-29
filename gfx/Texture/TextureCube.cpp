#include "TextureCube.h"


TextureCube :: TextureCube(const unsigned w,const unsigned h,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<std::vector<MipmapLevel> >& texdata,const unsigned target,
			const std::string& texname,const bool genMipmaps)
			:Texture(w,ifmt,pfmt,dtype,target,texname,genMipmaps),_height(h)
{
	_type = TEXCUBE;
	assert(texdata.size() == 6);

	bind();
	for(size_t face = 0;face<6;++face)
	{
		_maxMiplevel = GLuint(texdata[face].size() - 1);

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
				const unsigned char * tmp_data = (unsigned char *)(texdata[face][miplevel].data);
				const double adjust = 1.0 / pow(2.0,int(miplevel));
				unsigned tmp_w = unsigned(_width * adjust),
						 tmp_h = unsigned(_height * adjust);
				glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + unsigned(face),
										miplevel,_internalFormat,tmp_w,tmp_h,0,texdata[face][miplevel].size,tmp_data);
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
				const unsigned char * tmp_data = (unsigned char *)(texdata[face][miplevel].data);
				const double adjust = 1.0 / pow(2.0,int(miplevel));
				unsigned tmp_w = unsigned(_width * adjust),
						 tmp_h = unsigned(_height * adjust);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + unsigned(face),
							miplevel,_internalFormat,tmp_w,tmp_h,0,_pixelFormat,_datatype,tmp_data);
				++miplevel;
			}
			glTexParameteri(_target,GL_GENERATE_MIPMAP,GL_FALSE);
		}
	}
	setParam(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	setParam(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	if(_maxMiplevel)
		setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	else
		setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	setParam(GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	// Compute bytes per texel

	GLint vals[6];
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_TEXTURE_RED_SIZE,&vals[0]);
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_TEXTURE_GREEN_SIZE,&vals[1]);
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_TEXTURE_BLUE_SIZE,&vals[2]);
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_TEXTURE_ALPHA_SIZE,&vals[3]);
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_TEXTURE_LUMINANCE_SIZE,&vals[4]);
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_TEXTURE_INTENSITY_SIZE,&vals[5]);
	_bytesPerTexel += vals[0];
	_bytesPerTexel += vals[1];
	_bytesPerTexel += vals[2];
	_bytesPerTexel += vals[3];
	_bytesPerTexel += vals[4];
	_bytesPerTexel += vals[5];
	_bytesPerTexel >>= 3;
}

const GLuint TextureCube :: dataSize(const int miplevel) const
{
	bind();
	GLint size = 0;
	if(_isCompressed)
	{
		glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X,miplevel,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&size);
	}
	else
	{
		const double adjust = 1.0 / pow(2.0,miplevel);
		size = GLuint(_width*_height*adjust*adjust*_bytesPerTexel)*6;
	}
	return size;
}

void TextureCube :: ulData(const void * buffer,const CubemapFace face,const GLuint miplevel) const
{
	bind();
	assert(!_isCompressed);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,miplevel,0,0,_width,_height,_internalFormat,_datatype,buffer);
}

void TextureCube :: dlData(void * buffer,const CubemapFace face,const GLuint miplevel) const
{
	bind();
	if(_isCompressed)
	{
		glGetCompressedTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,miplevel,buffer);
	}
	else
		glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,miplevel,_pixelFormat,_datatype,buffer);
}

TEX_header TextureCube :: getTEXheader() const
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