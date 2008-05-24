#include "GameOverRenderer.h"
#include "RenderEngine.h"
//#include "../gfx/Shaders/ShaderManager.h"
//#include "../gfx/Texture/Texture.h"
//#include "../gfx/Texture/TextureIO.h"
//#include <gl/glee.h>
//#include <gl/glu.h>

GameOverRenderer::GameOverRenderer()
{
	//m_texture = TextureIO::instance()->getTexture("gameOverScreen.dds");
}

GameOverRenderer :: ~GameOverRenderer()
{
}


void GameOverRenderer :: render(Graphics& g) const
{
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
