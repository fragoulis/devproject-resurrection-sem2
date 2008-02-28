#pragma once

#include "textureMgr.h"
#include "texture.h"
#include <string>
#include <vector>
#include <iostream>
#include "../../gfxutils/Misc/utils.h"

/*
	Texture3D : 
	class for 3D textures
*/
class Texture3D : public Texture
{
	GLuint	   _height,
			   _depth;

public : 
	Texture3D(const unsigned w,const unsigned h,const unsigned d,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<MipmapLevel>& texdata,const unsigned target,	
			const std::string& texname,const bool genMipmaps);

	void * operator new(const unsigned size);
	void operator delete(void * p);

	// Accessors
	const GLuint getHeight() const;
	const GLuint getDepth() const;
	const GLuint dataSize(const int miplevel=0) const;

	// Utils
	void ulData(const void * buffer,const GLuint miplevel = 0) const;
	void dlData(void * buffer,const GLuint miplevel = 0) const;
	TEX_header getTEXheader() const;
};

inline const GLuint Texture3D :: getHeight() const {return _height;}
inline const GLuint Texture3D :: getDepth() const {return _depth;}

inline void * Texture3D :: operator new(const unsigned size)
{
	return MemMgrAny<Texture3D>::instance()->allocate();
}

inline void Texture3D :: operator delete(void * p)
{
	MemMgrAny<Texture3D>::instance()->free(reinterpret_cast<Texture3D *>(p));
}


