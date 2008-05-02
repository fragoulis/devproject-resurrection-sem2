#include "BandPostProc.h"
#include "RenderEngine.h"
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/FBO/FramebufferObject.h"
#include "../gfx/Shaders/ShaderManager.h"

#include <vector>

using namespace std;

BandPostProc :: BandPostProc()
{
}

BandPostProc :: ~BandPostProc()
{
}

Texture * BandPostProc :: process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const
{
	CHECK_GL_ERROR();
	fbo.AttachTexture(GL_TEXTURE_2D,output->getId(),GL_COLOR_ATTACHMENT0_EXT);
	bool res = fbo.IsValid();
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT);

	ShaderManager::instance()->begin("bandPassShader");
	input->bind(0);
	ShaderManager::instance()->setUniform1i("sourceTex",0);

	RenderEngine::drawTexturedQuad(Vector3(0,0,0),
								   Vector3(1,0,0),
								   Vector3(0,1,0),
								   Vector2(0,0),
								   Vector2(1,1));

	fbo.Unattach(GL_COLOR_ATTACHMENT0_EXT);

	return output;
}