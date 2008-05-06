#pragma once

#include <string>
#include <assert.h>
#include <vector>

namespace CKLib {

enum CompressionFmt_t
{
	NONE = 0,
	GENERIC,
	DXT1,
	DXT3,
	DXT5,
};

/*
	TextureIO : 
	Singleton class for I/O handling of textures.
	also used to retrieve loaded textures.
*/

class Texture;
class ParserSection;
class TextureIO
{
	std::string	_textureDir;
	bool		_loadAsRect,
				_saveMipmaps,
				_genMipmaps,
				_normalizeFloat;		// Normalizes float data when saving floating point textures
										// use min & max values in tex data

	std::vector<Texture *> _textureTemplates;


	static bool		   _init;
	static TextureIO * _instance;

	// prohibited funcs
	TextureIO(const ParserSection * parsec);			// ctor
	~TextureIO();										// dtor
	TextureIO(const TextureIO& tio){}					// copy ctor
	void operator = (const TextureIO& tio){}			// assign op

	// internal func
	void _parseConfiguration(const ParserSection * parsec);		// parses options

	// Load funcs
	Texture * _loadSGI(const std::string& fname);
	Texture * _loadTEX(const std::string& fname);
	Texture * _loadDDS(const std::string& fname);
	Texture * _loadBMP(const std::string& fname);

	// Saving funcs
	bool _saveSGI(const Texture * tex, const std::string& fname);	// mipmaps always set to false for SGI
	bool _saveTEX(const Texture * tex, const std::string& fname);
	bool _saveDDS(const Texture * tex, const std::string& fname);	// Won't be implemented anytime soon
	bool _saveBMP(const Texture * tex, const std::string& fname);	// Won't be implemented anytime soon

public :
	bool saveImage(const Texture * tex, 
						  const std::string& fname);
	bool saveImageFullPath(const Texture * tex, 
						  const std::string& fname);

	Texture * loadImage(const std::string& fname);
	Texture * loadImageFullPath(const std::string& fname);

	void setOptionLoadAsRect(const bool opt);
	void setOptionSaveMipmaps(const bool opt);
	void setOptionGenMipmaps(const bool opt);
	void setOptionNormalizeFloat(const bool opt);

	bool getOptionLoadAsRect() const;
	bool getOptionSaveMipmaps() const;
	bool getOptionGenMipmaps() const;
	bool getOptionNormalizeFloat() const;

	Texture * getTexture(const std::string& fname) ;// Should search a texture, or try to load it
	const std::string& getTextureDir() const;

	static void init(const ParserSection * parsec);
	static void destroy();
	static TextureIO * instance();
};

#include <Texture/TextureIO.inl>

}