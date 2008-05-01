#include "GrainPostProc.h"
#include "RenderEngine.h"
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/Shaders/ShaderManager.h"

#include <vector>

using namespace std;

GrainPostProc :: GrainPostProc()
{
	m_noise = TextureIO::instance()->getTexture("NoiseVolume.dds");
	m_rand = TextureIO::instance()->getTexture("Random3D.dds");
}

GrainPostProc :: ~GrainPostProc()
{
}

Texture * GrainPostProc :: process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const
{
	fbo.AttachTexture(GL_TEXTURE_2D,output->getId(),GL_COLOR_ATTACHMENT0_EXT);
	glClear(GL_COLOR_BUFFER_BIT);

	ShaderManager::instance()->begin("disturbedShader");
	input->bind(0);
	ShaderManager::instance()->setUniform1i("Image",0);
	m_noise->bind(1);
	ShaderManager::instance()->setUniform1i("Noise",1);
	m_rand->bind(2);
	ShaderManager::instance()->setUniform1i("Rand",2);
	const GLfloat distortionFreq = 5.7f;
	ShaderManager::instance()->setUniform1fv("distortionFreq", &distortionFreq);
	const GLfloat distortionScale = 6.0f;
	ShaderManager::instance()->setUniform1fv("distortionScale", &distortionScale);
	const GLfloat distortionRoll = 0.4f;
	ShaderManager::instance()->setUniform1fv("distortionRoll", &distortionRoll);
	const GLfloat interference = 0.49f;
	ShaderManager::instance()->setUniform1fv("interference", &interference);
	ShaderManager::instance()->setUniform1fv("time_0_X", &additive_time);

	RenderEngine::drawTexturedQuad(Vector3(0,0,0),
								   Vector3(1,0,0),
								   Vector3(0,1,0),
								   Vector2(0,0),
								   Vector2(1,1));

	fbo.Unattach(GL_COLOR_ATTACHMENT0_EXT);

	return output;
}