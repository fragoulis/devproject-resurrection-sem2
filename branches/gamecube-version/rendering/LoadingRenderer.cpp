#include "LoadingRenderer.h"
#include "RenderEngine.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/Texture.h"
#include "../gfxutils/VA/VATTable.h"
#include "../gfx/Camera.h"

#include <iostream>
using namespace std;


LoadingRenderer :: LoadingRenderer()
{
	//m_texture = TextureIO::instance()->getTexture("loadingScreen_alt.dds");
	TextureMgr::safeInstance().loadPalette("loadingScreen.tpl", "loadingScreenTPL.txt");
	m_texture = TextureMgr::instance().getTexture("loading");

	Camera::load2D();

    GXSetNumChans(1);  // Enable light channel; by default = vertex color

	GXSetVtxDescv(VATTable::getVDL(1));

	m_texture->bind();

    //  Set the Texture Environment (Tev) Mode for stage 0
    //  GXInit sets default of 1 TexCoordGen
    //  Default TexCoordGen is texcoord(n) from tex(n) with 2x4 identity mtx
    //  Default number of tev stages is 1
    //  Default stage0 uses texcoord0, texmap0, color0a0
    //  Only need to change the tevop
    GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);


    GXColor black = {0, 0, 0, 0};
    GXSetCopyClear(black, 0x00ffffff);
}

LoadingRenderer :: ~LoadingRenderer()
{
}


void LoadingRenderer :: render(Graphics& g) const
{
	Camera::activate2D();

	//GXBegin(GX_QUADS, GX_VTXFMT1, 4);
	//{
	//	GXPosition3s16(0, 480, 0);
	//	//GXColor1u32(u32(0xff0000ff));
	//	GXTexCoord2u8(0, 0);

	//	GXPosition3s16(640, 480, 0);
	//	//GXColor1u32(u32(0x00ff00ff));
	//	GXTexCoord2u8(1, 0);

	//	GXPosition3s16(640, 0, 0);
	//	//GXColor1u32(u32(0x0000ffff));
	//	GXTexCoord2u8(1, 1);

	//	GXPosition3s16(0, 0, 0);
	//	//GXColor1u32(u32(0x0000ffff));
	//	GXTexCoord2u8(0, 1);
	//}
	//GXEnd();

	RenderEngine::drawTexturedRectangle(60, 0, 512, 512);




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

	//// Render loading screen
	///*glDisable(GL_TEXTURE_2D);
	//glEnable(GL_COLOR_MATERIAL);
	//glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	//glBegin(GL_QUADS);
	//{
	//	glVertex3f(-1.0f, -1.0f, 0.0f);
	//	glVertex3f(1.0f, -1.0f, 0.0f);
	//	glVertex3f(1.0f, 1.0f, 0.0f);
	//	glVertex3f(-1.0f, 1.0f, 0.0f);
	//}
	//glEnd();
	//glEnable(GL_TEXTURE_2D);
	//glDisable(GL_COLOR_MATERIAL);*/


	///*ShaderManager::instance()->begin("blitShader");
	//m_texture->bind(0);
	//CHECK_GL_ERROR();
	//ShaderManager::instance()->setUniform1i("tex",0);
	//CHECK_GL_ERROR();

	//RenderEngine::drawTexturedQuad(Vector3(0, 0, 0), Vector3((float) screenWidth, 0, 0), Vector3(0, (float) screenHeight, 0), Vector2(0,0), Vector2(1,1));

	//ShaderManager::instance()->end();*/


	//ShaderManager::instance()->begin("blitShader");
	//m_texture->bind(0);
	//RenderEngine::drawTexturedQuad(Vector3(0.0f, 0.0f, 0.0f), Vector3((float) screenWidth, 0.0f, 0.0f), Vector3(0.0f, (float) screenHeight, 0.0f), Vector2(0.0f,0.0f), Vector2(1.0f,1.0f));
	//ShaderManager::instance()->end();


	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
	//glMatrixMode(GL_MODELVIEW);
	////glEnable(GL_LIGHTING);
}

void LoadingRenderer :: update( float dt )
{
	// animate loading screen :)
}