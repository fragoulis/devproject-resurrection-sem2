#include "RenderEngine.h"
#include "IRenderer.h"
#include "WorldRenderer.h"
#include "LoadingRenderer.h"
#include <cassert>



RenderEngine :: RenderEngine()
{
}

RenderEngine :: ~RenderEngine()
{
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

IRenderer* RenderEngine :: createRenderer(const std::string& name) const
{
	if (name == "world") return new WorldRenderer();
	if (name == "loading") return new LoadingRenderer();
	return NULL;
}
