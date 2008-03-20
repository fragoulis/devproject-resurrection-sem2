//*****************************************************************************
//**
//**   File:               texture2D.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include "textureMgr.h"
#include "texture.h"
#include <string>
#include <vector>
#include <iostream>
#include "../../gfxutils/Misc/utils.h"

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

inline const GLuint Texture2D :: getHeight() const {return _height;}
inline const bool Texture2D :: isTexRect() const {return (_target == GL_TEXTURE_RECTANGLE_ARB);}

inline void * Texture2D :: operator new(const unsigned size)
{
	return MemMgrAny<Texture2D>::instance()->allocate();
}

inline void Texture2D :: operator delete(void * p)
{
	MemMgrAny<Texture2D>::instance()->free(reinterpret_cast<Texture2D *>(p));
}

