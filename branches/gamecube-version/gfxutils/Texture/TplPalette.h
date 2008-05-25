#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <dolphin.h>
#include <demo.h>

#include <vector>
#include <string>

class TplPalette
{
	std::vector<std::string> m_nameList;
	TEXPalettePtr m_palettePtr;
	std::string m_name;

public :
	
	TplPalette(const std::string& imgfile,const std::string& namefile);
	~TplPalette();

	const TEXHeader * getSurface(const unsigned i) const {return m_palettePtr->descriptorArray[i].textureHeader;}
	unsigned getSurfaceNum() const {return m_palettePtr->numDescriptors;}
	const TEXPalettePtr getPalette() const {return m_palettePtr;}

	void getTexObj(GXTexObj& obj,const unsigned id) {TEXGetGXTexObjFromPalette(m_palettePtr,&obj,id);}

	const std::string& getName() const {return m_name;}
	const std::string getTexName(const unsigned i) const {if(i<m_nameList.size()) return m_nameList[i]; else return "";}
};

#endif