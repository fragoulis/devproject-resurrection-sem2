#include "ShockwavePostProc.h"
#include "RenderEngine.h"
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/Shaders/ShaderManager.h"

#include <vector>

#define SHOCK_MAX_TIME 1.0f

using namespace std;

ShockwavePostProc :: ShockwavePostProc()
:m_active(false),
m_startTime(0.0f)
{
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Respawned >(this);

	m_noiseTex = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getNoiseTexture());
}

ShockwavePostProc :: ~ShockwavePostProc()
{
}

Texture * ShockwavePostProc :: process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const
{
	if(!m_active)
		return input;
	if(m_startTime < 0.0f)
		*const_cast<float *>(&m_startTime) = additive_time;

	CHECK_GL_ERROR();
	fbo.AttachTexture(GL_TEXTURE_2D,output->getId(),GL_COLOR_ATTACHMENT0_EXT);
	bool res = fbo.IsValid();
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT);

	ShaderManager::instance()->begin("shockwaveShader");
	input->bind(0);
	ShaderManager::instance()->setUniform1i("sourceTex",0);
	m_noiseTex->bind(1);
	//ShaderManager::instance()->setUniform1i("noiseTex",1);
	ShaderManager::instance()->setUniform1f("time",(additive_time - m_startTime) / SHOCK_MAX_TIME);

	RenderEngine::drawTexturedQuad(Vector3(0,0,0),
								   Vector3(1,0,0),
								   Vector3(0,1,0),
								   Vector2(0,0),
								   Vector2(1,1));

	fbo.Unattach(GL_COLOR_ATTACHMENT0_EXT);

	if((additive_time - m_startTime) > SHOCK_MAX_TIME)
	{
		*const_cast<bool *>(&m_active) = false;
		*const_cast<float *>(&m_startTime) = -1.0f;
	}

	return output;
}

void ShockwavePostProc :: onEvent(Player_Spawned&)
{
	m_active = true;
}

void ShockwavePostProc :: onEvent(Player_Respawned&)
{
	m_active = true;
}