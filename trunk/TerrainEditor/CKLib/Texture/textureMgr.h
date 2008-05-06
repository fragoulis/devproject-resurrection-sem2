#ifndef _texture_h_
#define _texture_h_

#include <glee.h>
#include <vector>
#include <Misc/defines.h>
#include <assert.h>
#include <string>

const unsigned MAX_TEXTURE_UNITS = 16;
const unsigned MAX_TEXTURES = 1024;

namespace CKLib {

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
	Manager class for texture states in GL
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

#include <Texture/textureMgr.inl>

}

#endif