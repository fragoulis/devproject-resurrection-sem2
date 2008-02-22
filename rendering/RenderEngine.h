#pragma once
#include "../utility/ObjectReferenceMap.h"
#include <vector>
class Graphics;
class IRenderer;

class RenderEngine
{
public:
	static RenderEngine& instance() { return s_instance; }

	IRenderer* findRenderer(const std::string& name) { return m_renderers.findObjectReference(name); }

	void addRenderer(const std::string& name, IRenderer* r) { m_renderers.addObjectReference(r, name); }
	void removeRenderer(const std::string& name) { m_renderers.removeObjectReference(name); }

	void activateRenderer(IRenderer* r) { m_activeRenderers.push_back(r); }
	void activateRenderer(const std::string& name) { activateRenderer(findRenderer(name)); }

	void deactivateRenderer(IRenderer* r);
	void deactivateRenderer(const std::string& name) { deactivateRenderer(findRenderer(name)); }

	void render(Graphics& g) const;


private:
	typedef ObjectReferenceMap<IRenderer> RendererMap;
	RendererMap m_renderers;

	typedef std::vector<IRenderer*> RendererList;
	RendererList m_activeRenderers;


	static RenderEngine s_instance;

	RenderEngine();
	~RenderEngine();
};
