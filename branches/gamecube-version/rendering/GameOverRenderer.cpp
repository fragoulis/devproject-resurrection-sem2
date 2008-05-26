#include "GameOverRenderer.h"
#include "RenderEngine.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"
#include "../gfxutils/VA/VATTable.h"
#include "../gfx/Camera.h"

GameOverRenderer::GameOverRenderer()
{
	const std::string name = "gameOverScreen";
	TextureMgr::safeInstance().loadPalette(name + ".tpl", name + "TPL.txt");
	m_texture = TextureMgr::instance().getTexture(name);
}

GameOverRenderer :: ~GameOverRenderer()
{
}


void GameOverRenderer :: render(Graphics& g) const
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
	RenderEngine::drawTexturedRectangle(192, 176, 256, 128);



	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetZMode(TRUE, GX_LEQUAL, TRUE);





	//int viewPortDims[4];
	//RenderEngine::instance().getViewport(viewPortDims);
	//int screenWidth = viewPortDims[2];
	//int screenHeight = viewPortDims[3];

	////glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//	glLoadIdentity();
	//	gluOrtho2D(0, screenWidth, 0, screenHeight);
	//glMatrixMode(GL_MODELVIEW);
	////here goes all the 2D rendering
	//glPushMatrix();
	//	glLoadIdentity();


	///*ShaderManager::instance()->begin("blitShader");
	//m_texture->bind(0);
	//CHECK_GL_ERROR();
	//ShaderManager::instance()->setUniform1i("tex",0);
	//CHECK_GL_ERROR();

	//RenderEngine::drawTexturedQuad(Vector3(0, 0, 0), Vector3((float) screenWidth, 0, 0), Vector3(0, (float) screenHeight, 0), Vector2(0,0), Vector2(1,1));

	//ShaderManager::instance()->end();*/

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//ShaderManager::instance()->begin("hudShader");
	//m_texture->bind(0);
	//CHECK_GL_ERROR();
	//ShaderManager::instance()->setUniform1i("tex",0);
	//CHECK_GL_ERROR();
	//const GLfloat transparency = 1.0f;
	//ShaderManager::instance()->setUniform1fv("transparency", &transparency);
	//CHECK_GL_ERROR();
	//const GLfloat white[4] = {1.0f,1.0f,1.0f,1.0f};
	//ShaderManager::instance()->setUniform4fv("constantColor", white);
	//CHECK_GL_ERROR();

	//RenderEngine::drawTexturedQuad(Vector3((float) screenWidth/2 - 128, (float) screenHeight/2-64, 0.0f), Vector3(256, 0.0f, 0.0f), Vector3(0.0f, 128, 0.0f), Vector2(0.0f,0.0f), Vector2(1.0f,1.0f));
	//ShaderManager::instance()->end();

	//glDisable(GL_BLEND);

	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
	//glMatrixMode(GL_MODELVIEW);
	////glEnable(GL_LIGHTING);
}

void GameOverRenderer :: update(float dt)
{
}
