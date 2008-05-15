#ifndef __SURFACE_H_
#define __SURFACE_H_

#include <dolphin.h>
#include <charpipeline/texpalette.h>

class Texture
{
	GXTexObj m_texObj;

public :
	
	Texture(TEXPalettePtr& tpl, const unsigned id) {TEXGetGXTexObjFromPalette(tpl,&m_texObj,id);}
	~Texture();
	Texture();

	GXTexObj& getTexture() {return m_texObj;}
};

#endif