#pragma once

#include <Texture/textureMgr.h>
#include <Texture/texture.h>
#include <Texture/texture2D.h>
#include <string>
#include <vector>
#include <iostream>
#include <Misc/utils.h>

namespace CKLib {
/*
	TextureCube : 
	class for 2D Cubemap textures
*/

class TextureCube : public Texture
{
	GLuint	_height;

public : 
	TextureCube(const unsigned w,const unsigned h,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<std::vector<MipmapLevel> >& texdata,const unsigned target,
			const std::string& texname,const bool genMipmaps);

	void * operator new(const unsigned size);
	void operator delete(void * p);

	//Utils 
	void ulData(const void * buffer,const CubemapFace face,const GLuint miplevel = 0) const;
	void dlData(void * buffer,const CubemapFace face,const GLuint miplevel = 0) const;
	TEX_header getTEXheader() const;
	const GLuint dataSize(const int miplevel =0) const; 

	// Accessors 
	const GLuint getHeight() const;
	
};

#include <Texture/textureCube.inl>

}
