//*****************************************************************************
//**
//**   File:               TextureCube.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include "textureMgr.h"
#include "texture.h"
#include "texture2D.h"
#include <string>
#include <vector>
#include <iostream>
#include "../../gfxutils/Misc/utils.h"

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

inline const GLuint TextureCube :: getHeight() const {return _height;}

inline void * TextureCube :: operator new(const unsigned size)
{
	return MemMgrAny<TextureCube>::instance()->allocate();
}

inline void TextureCube :: operator delete(void * p)
{
	MemMgrAny<TextureCube>::instance()->free(reinterpret_cast<TextureCube *>(p));
}


