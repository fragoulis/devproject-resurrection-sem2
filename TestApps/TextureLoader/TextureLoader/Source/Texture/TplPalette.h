#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <dolphin.h>
#include <demo.h>

class TplPalette
{
	char     **	m_nameList;
	TEXPalettePtr m_palettePtr;

public :
	
	TplPalette(char * imgfile,char * namefile);
	~TplPalette();

	const TEXHeader * getSurface(const unsigned i) const {return m_palettePtr->descriptorArray[i].textureHeader;}
	unsigned getSurfaceNum() const {return m_palettePtr->numDescriptors;}
	const char * getName(const unsigned id) const {return 0;};			// FIXME!!!
	u32 getId(const char * name) const {return 0;};						// FIXME!!!
	const TEXPalettePtr getPalette() const {return m_palettePtr;}

	void getTexObj(GXTexObj& obj,const unsigned id) {TEXGetGXTexObjFromPalette(m_palettePtr,&obj,id);}
	void getTexObj(GXTexObj& obj,const char * name) {TEXGetGXTexObjFromPalette(m_palettePtr,&obj,getId(name));}
};

#endif