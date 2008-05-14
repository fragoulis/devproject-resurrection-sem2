#pragma once

#include <gl/glee.h>
#include <vector>
#include "../../gfxutils/Misc/defines.h"
#include <assert.h>
#include <string>

const unsigned MAX_TEXTURE_UNITS = 16;
const unsigned MAX_TEXTURES = 1024;



/*
	texture_info : 
	struct for a texture unit's needed information
*/
struct texture_info
{
	unsigned target;
	unsigned id;
};

/*
	Texture : 
	Manager class for texture state in GL
*/
class ParserSection;
class TextureMgr
{
private :

	texture_info	_textureInfo[MAX_TEXTURE_UNITS];

	// Statics
	
	static bool			_init;
	static TextureMgr * _instance;

	// internal funcs
	void _parseConfiguration(const ParserSection * parsec);	// parses options

	// prohibited funcs
	TextureMgr(const ParserSection * parsec);		// ctor
	~TextureMgr();																// dtor
	TextureMgr(const TextureMgr& mgr){}											// copy ctor
	void operator = (const TextureMgr& mgr){}									// assign op

public :
	void setTextureTarget(const unsigned& target,const unsigned& i);
	void setBoundTexture(const unsigned& id,const unsigned& i);
	void setTextureUnit(const unsigned& unit);	

	unsigned getTextureTarget(const unsigned& unit);
	unsigned getBoundTexture(const unsigned& unit);

	// Singleton Trio Stooges
	static void		init(const ParserSection * parsec);
	static TextureMgr * instance();
	static void		destroy();
};

inline TextureMgr * TextureMgr :: instance()
{
	assert(_init);
	return _instance;
}

inline void TextureMgr :: setTextureTarget(const unsigned& target,const unsigned& i)
{
	ASSERT(i < MAX_TEXTURE_UNITS);
	if(target != _textureInfo[i].target)
	{
		if(_textureInfo[i].target)
			glDisable(_textureInfo[i].target);
		if(target)
			glEnable(target);
		_textureInfo[i].target = target;		
	}
}

inline void TextureMgr :: setBoundTexture(const unsigned& id,const unsigned& i)
{
	ASSERT(i < MAX_TEXTURE_UNITS);
	if((id != _textureInfo[i].id) && _textureInfo[i].target)
	{
		// Tex Bind ++ / Debugging code
		glBindTexture(_textureInfo[i].target,id);
		_textureInfo[i].id = id;
	}
}

inline void TextureMgr :: setTextureUnit(const unsigned& unit)
{
	ASSERT(unit < MAX_TEXTURE_UNITS);
	glActiveTexture(GL_TEXTURE0 + unit);
}

inline unsigned TextureMgr :: getTextureTarget(const unsigned& unit)
{return _textureInfo[unit].target;}
inline unsigned TextureMgr :: getBoundTexture(const unsigned& unit)
{return _textureInfo[unit].id;}

