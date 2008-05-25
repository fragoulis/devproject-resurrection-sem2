#ifndef __SURFACE_H_
#define __SURFACE_H_

#include <dolphin.h>
#include <charpipeline/texpalette.h>
#include <string>

class TplPalette;
class Texture
{
	GXTexObj m_texObj;
	TplPalette * m_palette;
	unsigned  m_paletteID;
	std::string m_name;

public :
	
	Texture(TplPalette * palette, unsigned id,const std::string& name);
	~Texture();

	void bind(const GXTexMapID tid = GX_TEXMAP0) const;
	const std::string& getName() const {return m_name;}
	const TplPalette * parentPalette() const {return m_palette;}
};

#endif