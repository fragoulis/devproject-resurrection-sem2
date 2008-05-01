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

	tex = TextureIO::instance()->getTexture("NoiseVolume.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("Random3D.dds");
	m_textureList.push_back(tex);
	tex = TextureIO::instance()->getTexture("menuSelectionFrame.bmp");
	m_textureList.push_back(tex);

	m_state = state;

	m_selectable = true;
	m_visible = true;
	
	m_currentTime = 0.0f;
}

void MenuItem::update(float dt) {
	m_currentTime += dt;
}

void MenuItem::render(Graphics &g) const {
	if (m_visible) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		if (m_state == ITEM_STATE_UNSELECTED) {
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
		} else if (m_state == ITEM_STATE_SELECTED) {
			ShaderManager::instance()->begin("disturbedShader");
			m_textureList[(int) m_state]->bind(0);
			CHECK_GL_ERROR();
			ShaderManager::instance()->setUniform1i("Image",0);
			CHECK_GL_ERROR();
			m_textureList[2]->bind(1);
			CHECK_GL_ERROR();
			ShaderManager::instance()->setUniform1i("Noise",1);
			CHECK_GL_ERROR();
			m_textureList[3]->bind(2);
			CHECK_GL_ERROR();
			ShaderManager::instance()->setUniform1i("Rand",2);
			CHECK_GL_ERROR();
			const GLfloat distortionFreq = 5.7f;
			ShaderManager::instance()->setUniform1fv("distortionFreq", &distortionFreq);
			CHECK_GL_ERROR();
			const GLfloat distortionScale = 6.0f;
			ShaderManager::instance()->setUniform1fv("distortionScale", &distortionScale);
			CHECK_GL_ERROR();
			const GLfloat distortionRoll = 0.4f;
			ShaderManager::instance()->setUniform1fv("distortionRoll", &distortionRoll);
			CHECK_GL_ERROR();
			const GLfloat interference = 0.49f;
			ShaderManager::instance()->setUniform1fv("interference", &interference);
			CHECK_GL_ERROR();
			const GLfloat granularity = 1.5f;
			ShaderManager::instance()->setUniform1fv("granularity", &granularity);
			CHECK_GL_ERROR();
			const GLfloat distorsion = 1.0f;
			ShaderManager::instance()->setUniform1fv("distorsion", &distorsion);
			CHECK_GL_ERROR();
			ShaderManager::instance()->setUniform1fv("time_0_X", &m_currentTime);
			CHECK_GL_ERROR();
			//draw the scaled button
			RenderEngine::drawTexturedQuad(Vector3(m_posX-10, m_posY-10, 0), Vector3((float) m_width+40, 0, 0), Vector3(0, (float) m_height+20, 0), Vector2(0,0), Vector2(1,1));
			ShaderManager::instance()->end();

			//draw the selected button frame
			ShaderManager::instance()->begin("hudShader");
			m_textureList[4]->bind(0);
			CHECK_GL_ERROR();
			ShaderManager::instance()->setUniform1i("tex",0);
			CHECK_GL_ERROR();
			const GLfloat transparency = 1.0f;
			ShaderManager::instance()->setUniform1fv("transparency", &transparency);
			CHECK_GL_ERROR();
			const GLfloat white[4] = {1.0f,1.0f,1.0f,1.0f};
			ShaderManager::instance()->setUniform4fv("constantColor", white);
			CHECK_GL_ERROR();
			RenderEngine::drawTexturedQuad(Vector3(m_posX-10, m_posY-10, 0), Vector3((float) m_width+40, 0, 0), Vector3(0, (float) m_height+20, 0), Vector2(0,0), Vector2(1,1));
		}


		//RenderEngine::drawTexturedQuad(Vector3(m_posX, m_posY, 0), Vector3((float) m_width, 0, 0), Vector3(0, (float) m_height, 0), Vector2(0,0), Vector2(1,1));

		ShaderManager::instance()->end();

		glDisable(GL_BLEND);
	}
}