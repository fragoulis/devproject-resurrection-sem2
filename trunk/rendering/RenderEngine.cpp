#include "RenderEngine.h"
#include "IRenderer.h"
#include "WorldRenderer.h"
#include "LoadingRenderer.h"
#include <cassert>
#include <string>

#include "../gfxutils/ConfParser/ConfParser.h"
#include "../gfx/Texture/TextureMgr.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/ModelMgr.h"
#include "../gfx/VBO/VBOMgr.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"

using namespace std;


RenderEngine :: RenderEngine()
:m_confParser(0)
{
	setViewport(0,0,800,600);
}

RenderEngine :: ~RenderEngine()
{
}

void RenderEngine :: allocateResources(const std::string& configfile)
{
	m_confParser = new ConfParser(configfile);

	MemMgrRaw::init(m_confParser->getSection("MemManager:RawData"));
	TextureMgr::init(m_confParser->getSection("Texture"));
	ShaderManager::init(m_confParser->getSection("Shader"));
	VBOMgr::safeInstance().init(m_confParser->getSection("VBOSettings"));
	ModelMgr::safeInstance().init(m_confParser->getSection("ModelSettings"));
}

void RenderEngine :: freeResources()
{
	ModelMgr::destroy();
	VBOMgr::destroy();
	ShaderManager::destroy();
	TextureMgr::destroy();
	MemMgrRaw::destroy();

	if(m_confParser)
		delete m_confParser;
}

void RenderEngine :: activateRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r == NULL) {
		r = createRenderer(name);
		assert(r != NULL);
		addRenderer(name, r);
	}
	activateRenderer(r);
}

void RenderEngine :: deactivateRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r != NULL) deactivateRenderer(r);
}

void RenderEngine :: deactivateAllRenderers()
{
	m_activeRenderers.clear();
}

void RenderEngine :: loadRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r == NULL) {
		r = createRenderer(name);
		assert(r != NULL);
		addRenderer(name, r);
	}
}

void RenderEngine :: unloadRenderer(const std::string& name)
{
	IRenderer* r = findRenderer(name);
	if (r != NULL) {
		deactivateRenderer(r);
		removeRenderer(name);
		delete r;
	}
}

void RenderEngine::unloadAllRenderers()
{
	struct RendererDeleter
	{
		void operator () (const std::string&, IRenderer* r) { delete r; }
	};
	m_renderers.visitObjects(RendererDeleter());
}


void RenderEngine :: render(Graphics &g) const
{
	for (RendererList::const_iterator i = m_activeRenderers.begin(); i != m_activeRenderers.end(); ++i)
		(*i)->render(g);
}

void RenderEngine :: update ( float dt )
{
	for (RendererList::const_iterator i = m_activeRenderers.begin(); i != m_activeRenderers.end(); ++i)
		(*i)->update(dt);
}

IRenderer* RenderEngine :: createRenderer(const std::string& name) const
{
	if (name == "world") return new WorldRenderer();
	if (name == "loading") return new LoadingRenderer();
	return NULL;
}

const ParserSection * RenderEngine :: getParserSection(const std::string& secname) const
{
	return m_confParser->getSection(secname);
}

void RenderEngine :: setViewport(int x,int y,int width, int height)
{
	m_viewport[0] = x;
	m_viewport[1] = y;
	m_viewport[2] = width;
	m_viewport[3] = height;
	glViewport(m_viewport[0],m_viewport[1],m_viewport[2],m_viewport[3]);
}

void RenderEngine :: getViewport(int vp[4]) const
{
	vp[0] = m_viewport[0];
	vp[1] = m_viewport[1];
	vp[2] = m_viewport[2];
	vp[3] = m_viewport[3];
}