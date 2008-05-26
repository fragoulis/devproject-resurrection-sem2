#include "InstructionPage.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"

void InstructionPage::init(int screenWidth, int screenHeight)
{
	const std::string texname = "instructionsBg";
	TextureMgr::safeInstance().loadPalette(texname + ".tpl", texname + "TPL.txt");
	m_backgroundImage = TextureMgr::instance().getTexture(texname);

	m_hasBackground = true;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}
