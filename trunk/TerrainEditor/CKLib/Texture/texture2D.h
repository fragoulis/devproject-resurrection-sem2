#ifndef _texture2D_h_
#define _texture2D_h_

#include <Texture/textureMgr.h>
#include <Texture/texture.h>
#include <string>
#include <vector>
#include <iostream>
#include <Misc/utils.h>

namespace CKLib {
/*
	Texture2D : 
	class for 2D textures
*/
class Texture2D : public Texture
{
	GLuint	   _height;

public : 
	Texture2D(const unsigned w,const unsigned h,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<MipmapLevel>& texdata,const unsigned target,	
			const std::string& texname,const bool genMipmaps,const bool loadAsRect = false);

	void * operator new(const unsigned size);
	void operator delete(void * p);

	// Accessors
	const GLuint getHeight() const;
	const bool isTexRect() const;
	const GLuint dataSize(const int miplevel=0) const;

	// Utils
	void ulData(const void * buffer,const GLuint miplevel = 0) const;
	void dlData(void * buffer,const GLuint miplevel = 0) const;
	TEX_header getTEXheader() const;
};

#include <Texture/texture2D.inl>

}

#endif