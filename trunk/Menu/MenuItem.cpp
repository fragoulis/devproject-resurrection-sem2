#include "MenuItem.h"
#include "../rendering/RenderEngine.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Texture/TextureIO.h"

MenuItem::~MenuItem(){

}

void MenuItem::init(float posX, float posY, int width, int height, const string &unselectedTexture, const string &selectedTexture, ItemState state) {
	m_posX = posX;
	m_posY = posY;
	m_width = width;
	m_height = height;

	Texture *tex = TextureIO::instance()->getTexture(unselectedTexture);
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture(selectedTexture);
	m_textureList.push_back(tex);

	m_state = state;
}

void MenuItem::update(float dt) {

}

void MenuItem::render(Graphics &g) const {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderManager::instance()->begin("hudShader");
	m_textureList[(int) m_state]->bind(0);
	CHECK_GL_ERROR();
	ShaderManager::instance()->setUniform1i("tex",0);
	CHECK_GL_ERROR();
	const GLfloat transparency = 1.0f;
	ShaderManager::instance()->setUniform1fv("transparency", &transparency);
	CHECK_GL_ERROR();
	const GLfloat white[4] = {1.0f,1.0f,1.0f,1.0f};
	ShaderManager::instance()->setUniform4fv("constantColor", white);
	CHECK_GL_ERROR();


	RenderEngine::drawTexturedQuad(Vector3(m_posX, m_posY, 0), Vector3((float) m_width, 0, 0), Vector3(0, (float) m_height, 0), Vector2(0,0), Vector2(1,1));

	ShaderManager::instance()->end();

	glDisable(GL_BLEND);
}