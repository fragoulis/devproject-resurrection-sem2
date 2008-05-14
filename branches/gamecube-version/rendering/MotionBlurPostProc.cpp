#include "MotionBlurPostProc.h"
#include "RenderEngine.h"
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/Shaders/ShaderManager.h"

#include <vector>

using namespace std;

MotionBlurPostProc :: MotionBlurPostProc()
:m_filled(0),
m_index(0)
{
	vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel(0,0));

	for(unsigned i=0;i<MB_STORED;++i)
		m_stored[i] = new Texture2D(1024,768,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"prev surface",false,false);
}

MotionBlurPostProc :: ~MotionBlurPostProc()
{
	for(unsigned i=0;i<MB_STORED;++i)
		delete m_stored[i];
}

Texture * MotionBlurPostProc :: process(Texture * input,Texture * output, FramebufferObject& fbo,const float additive_time) const
{
	CHECK_GL_ERROR();
	fbo.AttachTexture(GL_TEXTURE_2D,output->getId(),GL_COLOR_ATTACHMENT0_EXT);
	bool res = fbo.IsValid();
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT);

	const float percent = 1.0f / (MB_STORED + 1.0f);

	ShaderManager::instance()->begin("additiveShader");
	ShaderManager::instance()->setUniform1i("tex",0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);

	unsigned start=m_index + MB_STORED - 1;
	for(int i=start;i>=int(m_index);--i)
	{
		const float p = 1.0 / (2.0*(2.0 + start-i));
		ShaderManager::instance()->setUniform1f("percentage",p);
		m_stored[i%MB_STORED]->bind();
		RenderEngine::drawTexturedQuad(Vector3(0,0,0),
									   Vector3(1,0,0),
									   Vector3(0,1,0),
									   Vector2(0,0),
									   Vector2(1,1));
	}
	ShaderManager::instance()->setUniform1f("percentage",0.5f);
	input->bind();
	RenderEngine::drawTexturedQuad(Vector3(0,0,0),
									   Vector3(1,0,0),
									   Vector3(0,1,0),
									   Vector2(0,0),
									   Vector2(1,1));

	glDisable(GL_BLEND);
 
	fbo.Unattach(GL_COLOR_ATTACHMENT0_EXT);

	fbo.AttachTexture(GL_TEXTURE_2D,input->getId(),GL_COLOR_ATTACHMENT1_EXT);
	fbo.AttachTexture(GL_TEXTURE_2D,m_stored[m_index]->getId(),GL_COLOR_ATTACHMENT0_EXT);
	
	glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBlitFramebufferEXT(0, 0, 1024, 768, 0, 0, 1024, 768,GL_COLOR_BUFFER_BIT  , GL_NEAREST);

	fbo.Unattach(GL_COLOR_ATTACHMENT0_EXT);
	fbo.Unattach(GL_COLOR_ATTACHMENT1_EXT);

	*(const_cast<unsigned *>(&m_index)) = (m_index + 1)%MB_STORED;

	return output;
}