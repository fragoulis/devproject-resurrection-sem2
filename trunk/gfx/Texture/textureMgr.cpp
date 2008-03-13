//*****************************************************************************
//**
//**   File:               textureMgr.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "textureMgr.h"
#include "../../gfxutils/MemManager/MemMgr_AnyData.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "TextureCube.h"

using namespace std;

bool TextureMgr :: _init = false;
TextureMgr * TextureMgr :: _instance(NULL);

// ctor - dtor

TextureMgr :: TextureMgr(const ParserSection * parsec)
{
	memset(_textureInfo,0,sizeof(texture_info)*MAX_TEXTURE_UNITS);
	_parseConfiguration(parsec);
}

TextureMgr :: ~TextureMgr()
{}

// internal Funcs

void TextureMgr :: _parseConfiguration(const ParserSection * parsec)
{
	// Nothing to set up here at the moment
}


// Static funcs

void TextureMgr :: init(const ParserSection * parsec)
{
	if(!_init)
	{
		_instance = new TextureMgr(parsec);	

		glPixelStorei (GL_UNPACK_ALIGNMENT , 1);
		glPixelStorei (GL_UNPACK_SKIP_ROWS , 0);
		glPixelStorei (GL_UNPACK_SKIP_PIXELS , 0);
		glPixelStorei (GL_UNPACK_ROW_LENGTH , 0);
		glPixelStorei (GL_UNPACK_SWAP_BYTES , 0);
		glPixelStorei (GL_PACK_ALIGNMENT , 1);

		TextureIO::init(parsec);
		// Memory allocation

		MemMgrAny<Texture1D>::init("Texture1D",parsec);
		MemMgrAny<Texture2D>::init("Texture2D",parsec);
		MemMgrAny<Texture3D>::init("Texture3D",parsec);
		MemMgrAny<TextureCube>::init("TextureCube",parsec);
		
		_init = true;
	}
}

void TextureMgr :: destroy()
{
	if(_init)
	{
		delete _instance;
		_instance = 0;
		MemMgrAny<Texture1D>::destroy();
		MemMgrAny<Texture2D>::destroy();
		MemMgrAny<Texture3D>::destroy();
		MemMgrAny<TextureCube>::destroy();
		TextureIO::destroy();
		_init = false;
	}
}