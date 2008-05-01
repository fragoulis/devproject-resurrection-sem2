#include "EdgePostProc.h"
#include "RenderEngine.h"
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/Shaders/ShaderManager.h"

#include <vector>

using namespace std;

EdgePostProc :: EdgePostProc(Texture * depth_buffer)
:m_depth(depth_buffer)
{
	vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel(0,0));
	m_edges = new Texture2D(depth_buffer->getWidth()/2,depth_buffer->getHeight()/2,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"edge surface",false,false);
}

EdgePostProc :: ~EdgePostProc()
{
	delete m_edges;
}

Texture * EdgePostProc :: process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const
{
	CHECK_GL_ERROR();
	fbo.AttachTexture(GL_TEXTURE_2D,m_edges->getId(),GL_COLOR_ATTACHMENT0_EXT);
	bool res = fbo.IsValid();
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT);

	ShaderManager::instance()->begin("edgeDetectShader");
	m_depth->bind(0);
	ShaderManager::instance()->setUniform1i("sourceTex",0);

	ShaderManager::instance()->setUniform1f("width",m_edges->getWidth());
	ShaderManager::instance()->setUniform1f("height",m_edges->getHeight());

	RenderEngine::drawTexturedQuad(Vector3(0,0,0),
								   Vector3(1,0,0),
								   Vector3(0,1,0),
								   Vector2(0,0),
								   Vector2(1,1));

	fbo.AttachTexture(GL_TEXTURE_2D,output->getId(),GL_COLOR_ATTACHMENT0_EXT);
	ShaderManager::instance()->begin("edgeAddShader");
	input->bind(0);
	ShaderManager::instance()->setUniform1i("sourceTex",0);
	m_edges->bind(1);
	ShaderManager::instance()->setUniform1i("edgeTex",1);

	RenderEngine::drawTexturedQuad(Vector3(0,0,0),
								   Vector3(1,0,0),
								   Vector3(0,1,0),
								   Vector2(0,0),
								   Vector2(1,1));

	fbo.Unattach(GL_COLOR_ATTACHMENT0_EXT);

	return output;
}