#include "Texture.h"
#include "TplPalette.h"

Texture :: Texture(TplPalette * palette, unsigned id,const std::string& name)
:m_palette(palette),
m_paletteID(id),
m_name(name)
{
	palette->getTexObj(m_texObj,id);
}

Texture :: ~Texture()
{	
}

void Texture :: bind(const GXTexMapID tid) const
{
	GXLoadTexObj(&m_texObj, tid);
}