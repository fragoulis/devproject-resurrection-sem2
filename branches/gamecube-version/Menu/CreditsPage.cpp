#include "CreditsPage.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"

void CreditsPage::init(int screenWidth, int screenHeight)
{
	const std::string texname = "creditsBg";
	TextureMgr::safeInstance().loadPalette(texname + ".tpl", texname + "TPL.txt");
	m_backgroundImage = TextureMgr::instance().getTexture(texname);
	
	m_hasBackground = true;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}
