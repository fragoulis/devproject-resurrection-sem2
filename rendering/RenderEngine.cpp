#include "RenderEngine.h"
#include "IRenderer.h"
#include <algorithm>


RenderEngine::RenderEngine()
{
}

RenderEngine::~RenderEngine()
{
}

void RenderEngine::deactivateRenderer(IRenderer* r)
{
	remove(m_activeRenderers.begin(), m_activeRenderers.end(), r);
}

void RenderEngine::render(Graphics &g) const
{
	for (RendererList::const_iterator i = m_activeRenderers.begin(); i != m_activeRenderers.end(); ++i)
		(*i)->render(g);
}
