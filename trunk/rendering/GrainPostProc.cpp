#include "GrainPostProc.h"
#include "RenderEngine.h"
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/Shaders/ShaderManager.h"

#include <vector>

using namespace std;

GrainPostProc :: GrainPostProc()
:m_adjTrigger(false)
{
	EventManager::instance().registerEventListener< Player_EnergyDrained >(this);
	m_noise = TextureIO::instance()->getTexture("NoiseVolume.dds");
	m_rand = TextureIO::instance()->getTexture("Random3D.dds");
}

GrainPostProc :: ~GrainPostProc()
{
}

Texture * GrainPostProc :: process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const
{
	if(m_adjTrigger)
	{
		*const_cast<float *>(&m_noiseAdj) = 1.0f;
		*const_cast<float *>(&m_adjStartTime) = additive_time;
		*const_cast<bool *>(&m_adjTrigger) = false;
	}
	else
	{
		if((additive_time - m_adjStartTime) > 0.2f)
			*const_cast<float *>(&m_noiseAdj) = 0.0f;
	}

	const float noise_adjustment = m_noiseAdj*abs((additive_time - m_adjStartTime) - 0.1f);

	fbo.AttachTexture(GL_TEXTURE_2D,output->getId(),GL_COLOR_ATTACHMENT0_EXT);
	glClear(GL_COLOR_BUFFER_BIT);

	ShaderManager::instance()->begin("disturbedShader");
	input->bind(0);
	ShaderManager::instance()->setUniform1i("Image",0);
	m_noise->bind(1);
	ShaderManager::instance()->setUniform1i("Noise",1);
	m_rand->bind(2);
	ShaderManager::instance()->setUniform1i("Rand",2);
	const GLfloat distortionFreq = 15.7f;
	ShaderManager::instance()->setUniform1fv("distortionFreq", &distortionFreq);
	const GLfloat distortionScale = 116.0f;
	ShaderManager::instance()->setUniform1fv("distortionScale", &distortionScale);
	const GLfloat distortionRoll = 111.4f;
	ShaderManager::instance()->setUniform1fv("distortionRoll", &distortionRoll);
	const GLfloat interference = 0.04f;//*(1.0f + noise_adjustment*10);
	ShaderManager::instance()->setUniform1fv("interference", &interference);
	const GLfloat granularity = 1.5f;
	ShaderManager::instance()->setUniform1fv("granularity", &granularity);
	const GLfloat distorsion = 0;
	ShaderManager::instance()->setUniform1fv("distorsion", &distorsion);
	ShaderManager::instance()->setUniform1f("time_0_X", 0.00008f*additive_time*(1.0f + noise_adjustment*1000));

	RenderEngine::drawTexturedQuad(Vector3(0,0,0),
								   Vector3(1,0,0),
								   Vector3(0,1,0),
								   Vector2(0,0),
								   Vector2(1,1));

	fbo.Unattach(GL_COLOR_ATTACHMENT0_EXT);

	return output;
}

void GrainPostProc :: onEvent(Player_EnergyDrained&)
{
	m_adjTrigger = true;

}