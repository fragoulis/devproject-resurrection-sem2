#include "TextureMgr.h"
#include "Texture.h"
#include "TplPalette.h"
#include "../../control/Gamecube/gc_new_delete.h"
#include "../Confparser/Parsersection.h"
#include <dolphin.h>

using namespace std;

Texture * TextureMgr :: getTexture(const std::string& texname)
{
	// search for the tex list for the name. 
	// If not found, try & load the file && search if there are 

	for(unsigned i=0;i<m_texList.size();++i)
	{
		if(m_texList[i]->getName() == texname)
			return m_texList[i];
	}
	ASSERTMSG("Texture %s not found!!\n",texname.c_str());
	return 0;
}

TplPalette * TextureMgr :: loadPalette(const std::string& palettefile,const std::string& namesfile)
{
	m_paletteList.push_back(new TplPalette(palettefile,namesfile));
	for(unsigned i=0;i<m_paletteList.back()->getSurfaceNum();++i)
		m_texList.push_back(new Texture(m_paletteList.back(),i,m_paletteList.back()->getTexName(i)));
	GXInvalidateTexAll();
	return m_paletteList.back();
}


void TextureMgr :: unloadPalette(const TplPalette * tpl)
{
	for(unsigned i=0;i<m_paletteList.size();++i)
	{
		if(m_paletteList[i] == tpl)
		{
			for(vector<Texture *>::iterator it = m_texList.begin();
				it!= m_texList.end();
				)
				{
					if((*it)->parentPalette() == tpl)
					{
						delete (*it);
						it = m_texList.erase(it);
					}
					else
						++it;
				}

			TplPalette * tmp = m_paletteList[i];
			m_paletteList[i] = m_paletteList.back();
			m_paletteList.back() = tmp;
			delete m_paletteList.back();
			m_paletteList.pop_back();
			GXInvalidateTexAll();
			return;
		}
	}
	ASSERTMSG("Palette %s not found!!\n",tpl->getName());
}

void TextureMgr :: unloadPalette(const std::string& tpl)
{
	for(unsigned i=0;i<m_paletteList.size();++i)
	{
		if(m_paletteList[i]->getName() == tpl)
		{
			for(vector<Texture *>::iterator it = m_texList.begin();
				it!= m_texList.end();
				)
				{
					if((*it)->parentPalette()->getName() == tpl)
					{
						delete (*it);
						it = m_texList.erase(it);
					}
					else
						++it;
				}
			TplPalette * tmp = m_paletteList[i];
			m_paletteList[i] = m_paletteList.back();
			m_paletteList.back() = tmp;
			delete m_paletteList.back();
			m_paletteList.pop_back();
			GXInvalidateTexAll();
			return;
		}
	}
	ASSERTMSG("Palette %s not found!!\n",tpl->getName());
}

void TextureMgr :: init(const ParserSection * parsec)
{
	m_texDir = parsec->getVal("TextureDir");
	OSReport("Texture directory set to %s...\n",m_texDir.c_str());
}


TextureMgr :: ~TextureMgr()
{	
	for(unsigned i=0;i<m_texList.size();++i)
		delete m_texList[i];
	m_texList.clear();
	for(unsigned i=0;i<m_paletteList.size();++i)
		delete m_paletteList[i];
	m_paletteList.clear();
	GXInvalidateTexAll();
}