#include "LevelCompleteRenderer.h"
#include "RenderEngine.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"
#include "../gfxutils/VA/VATTable.h"
#include "../gfx/Camera.h"

LevelCompleteRenderer::LevelCompleteRenderer()
{
	const std::string name = "levelComplete";
	TextureMgr::safeInstance().loadPalette(name + ".tpl", name + "TPL.txt");
	m_texture = TextureMgr::instance().getTexture(name);
}

LevelCompleteRenderer :: ~LevelCompleteRenderer()
{
}


void LevelCompleteRenderer :: render(Graphics& g) const
{
	Camera::activate2D();
	GXSetZMode(FALSE, GX_ALWAYS, FALSE);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetVtxDescv(VATTable::getVDL(1));
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	// Set texture alpha channel to texture red channel
	GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_RED);
	GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP1);

	m_texture->bind();
	RenderEngine::drawTexturedRectangle(64, 176, 512, 128);



	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetZMode(TRUE, GX_LEQUAL, TRUE);
}

void LevelCompleteRenderer :: update(float dt)
{
}
