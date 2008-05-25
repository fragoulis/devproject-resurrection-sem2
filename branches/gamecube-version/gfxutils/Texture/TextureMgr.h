#ifndef __TextureMgr_h__
#define __TextureMgr_h__

#include "../../utility/Singleton.h"

#include <vector>
#include <string>

class Texture;
class TplPalette;
class ParserSection;
class TextureMgr : public Singleton<TextureMgr>
{
	
	std::vector<Texture *> m_texList;
	std::vector<TplPalette *> m_paletteList;
	std::string m_texDir;

public:
	~TextureMgr();

	Texture * getTexture(const std::string& texname);
	TplPalette * loadPalette(const std::string& palettefile,const std::string& namesfile);

	void init(const ParserSection * parsec);
	void unloadPalette(const TplPalette * tpl);
	void unloadPalette(const std::string& tpl);

	const std::string& getTextureDir() {return m_texDir;}
};

#endif