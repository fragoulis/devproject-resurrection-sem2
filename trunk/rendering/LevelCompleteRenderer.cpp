#include "LevelCompleteRenderer.h"
#include "../rendering/RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"
#include "gl/glu.h"
#include <gl/glee.h>

const float POS_X = 10.0f;
const float POS_Y = 10.0f;
const float WIDTH = 100.0f;
const float HEIGHT = 100.0f;

LevelCompleteRenderer::LevelCompleteRenderer()
{
	m_texture = TextureIO::instance()->getTexture("menuExit.bmp");

	int viewPortDims[4];
	RenderEngine::instance().getViewport(viewPortDims);
	int m_screenWidth = viewPortDims[2];
	int m_screenHeight = viewPortDims[3];
}

LevelCompleteRenderer::~LevelCompleteRenderer()
{
}

void LevelCompleteRenderer :: render(Graphics& g) const
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, m_screenWidth, 0, m_screenHeight);
	glMatrixMode(GL_MODELVIEW);
	//here goes all the 2D rendering
	glPushMatrix();
		glLoadIdentity();



	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderManager::instance()->begin("hudShader");
	m_texture->bind(0);
	CHECK_GL_ERROR();
	ShaderManager::instance()->setUniform1i("tex", 0);
	CHECK_GL_ERROR();
	const GLfloat transparency = 1.0f;
	ShaderManager::instance()->setUniform1fv("transparency", &transparency);
	CHECK_GL_ERROR();
	const GLfloat white[4] = {1.0f,1.0f,1.0f,1.0f};
	ShaderManager::instance()->setUniform4fv("constantColor", white);
	CHECK_GL_ERROR();

	RenderEngine::drawTexturedQuad(Vector3(POS_X, POS_Y, 0), Vector3(WIDTH, 0, 0), Vector3(0, HEIGHT, 0), Vector2(0,0), Vector2(1,1));

	ShaderManager::instance()->end();

	glDisable(GL_BLEND);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

void LevelCompleteRenderer :: update(float dt)
{
}
