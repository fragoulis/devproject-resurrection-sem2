//*****************************************************************************
//**
//**   File:               Texture1D.h
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
	Texture1D : 
	class for 1D textures
*/
class Texture1D : public Texture
{

public : 
	Texture1D(const unsigned w,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,
			const std::vector<MipmapLevel>& texdata,const unsigned target,	
			const std::string& texname,const bool genMipmaps,const bool loadAsRect = false);

	void * operator new(const unsigned size);
	void operator delete(void * p);

	// Accessors
	const GLuint dataSize(const int miplevel=0) const;

	// Utils
	void ulData(const void * buffer,const GLuint miplevel = 0) const;
	void dlData(void * buffer,const GLuint miplevel = 0) const;
	TEX_header getTEXheader() const;
};

inline void * Texture1D :: operator new(const unsigned size)
{
	return MemMgrAny<Texture1D>::instance()->allocate();
}

inline void Texture1D :: operator delete(void * p)
{
	MemMgrAny<Texture1D>::instance()->free(reinterpret_cast<Texture1D *>(p));
}

